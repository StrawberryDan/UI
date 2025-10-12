#include "TextNodeRenderer.hpp"
#include "TextNodeRenderer.hpp"

#include "Strawberry/UI/TextNode.hpp"
#include "Strawberry/Vulkan/Resource/Buffer.hpp"
#include "Strawberry/Vulkan/Resource/Buffer.hpp"


static uint8_t VERTEX_SHADER[] = {
#include "TextNodeRenderer.vert.bin"
};

static uint8_t FRAGMENT_SHADER[] = {
#include "TextNodeRenderer.frag.bin"
};


namespace Strawberry::UI
{
	size_t DEFAULT_GLYPH_BUFFER_SIZE = 1024;


	TextNodeRenderer::TextNodeRenderer(Vulkan::Framebuffer& framebuffer, unsigned int subpassIndex)
		: mPipelineLayout([&]() {
			  return Vulkan::PipelineLayout::Builder(framebuffer.GetDevice())
					 .WithDescriptor(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
									 VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
					 .WithDescriptor(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
									 VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
					 .WithDescriptor(1, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
									 VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
									 1)
					 .WithDescriptor(1, VK_DESCRIPTOR_TYPE_SAMPLER,
									 VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
									 1)
					 .Build();
		  }())
		  , mGraphicsPipeline([&]() -> Vulkan::GraphicsPipeline {
			  return Vulkan::GraphicsPipeline::Builder(mPipelineLayout, framebuffer.GetRenderPass(), subpassIndex)
					 .WithInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
					 .WithViewport(framebuffer, false)
					 .WithAlphaColorBlending()
					 .WithRasterization(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE)
					 .WithShaderStages(Vulkan::Shader::Compile(framebuffer.GetDevice(), VERTEX_SHADER).Unwrap(),
									  Vulkan::Shader::Compile(framebuffer.GetDevice(), FRAGMENT_SHADER).Unwrap())
					 .WithDepthTesting()
					 .Build();
		  }())
		  , mGPUGlyphBuffer(Vulkan::Buffer::Builder(framebuffer.GetDevice(), Vulkan::MemoryTypeCriteria::HostVisible())
							.WithSize(sizeof(Glyph) * DEFAULT_GLYPH_BUFFER_SIZE)
							.WithUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
							.Build())
		  , mGlyphBufferDescriptorSet(framebuffer.GetDevice(), mPipelineLayout.GetSetLayout(0))
		  , mConstantsBuffer(
							 Vulkan::Buffer::Builder(framebuffer.GetDevice(),
													 Vulkan::MemoryTypeCriteria::HostVisible())
							 .WithSize(sizeof(Core::Math::Mat4f) + sizeof(Core::Math::Vec2u))
							 .WithUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT |
										VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
							 .Build()
							)
	{
		mGlyphBufferDescriptorSet.SetStorageBuffer(0, 0, mGPUGlyphBuffer);
	}


	void TextNodeRenderer::Submit(uint32_t drawIndex, const TextNode& node)
	{
		const LoadedFont& font = mFontMaps.at(node.GetFont());

		Core::Math::Vec2u glyphCursor;
		for (char32_t codepoint: node.GetString())
		{
			FontMap::GlyphAddress glyphAddress = font.cpuFontMap.GetGlyphAddress(codepoint).Unwrap();
			mGlyphBuffer.emplace_back(
									  Glyph{
										  .font = node.GetFont(),
										  .position = node.GetPosition().AsType<unsigned>() + glyphCursor,
										  .size = glyphAddress.extent,
										  .glyphAddressPageIndex = glyphAddress.pageIndex,
										  .glyphAddressCoordinate = glyphAddress.offset
									  }
									 );

			glyphCursor[0] += glyphAddress.extent[0];
		}
	}


	void TextNodeRenderer::Render(Vulkan::CommandBuffer& commandBuffer, Core::Math::Mat4f projectionMatrix)
	{
		Core::IO::DynamicByteBuffer glyphBufferContents;

		if (!mGlyphBuffer.empty())
		{
			mConstantsBuffer.SetData(Core::IO::DynamicByteBuffer::FromObjects(projectionMatrix, mFontMaps.at(mGlyphBuffer[0].font).cpuFontMap.GetPageSize()));

			for (auto& glyph: mGlyphBuffer)
			{
				glyphBufferContents.Push(glyph.position);
				glyphBufferContents.Push(glyph.size);
				glyphBufferContents.Push(glyph.glyphAddressPageIndex);
				glyphBufferContents.Push<uint32_t>(0);
				glyphBufferContents.Push(glyph.glyphAddressCoordinate);
			}

			mGPUGlyphBuffer.SetData(glyphBufferContents);

			commandBuffer.BindPipeline(mGraphicsPipeline);
			commandBuffer.BindDescriptorSet(mGraphicsPipeline, 0, mGlyphBufferDescriptorSet);
			commandBuffer.BindDescriptorSet(mGraphicsPipeline, 1,
											mFontMaps.at(mGlyphBuffer.front().font).descriptorSet);
			commandBuffer.Draw(6, static_cast<unsigned int>(mGlyphBuffer.size()));
			mGlyphBuffer.clear();
		}
	}


	void TextNodeRenderer::LoadFont(Vulkan::Queue& queue, std::string ID, FontFace&& fontFace)
	{
		FontMap    fontMap(fontFace);
		GPUFontMap gpuFontMap = fontMap.CopyToGPU(queue);

		auto& insertedFontmap = mFontMaps.insert_or_assign(ID, LoadedFont
														   {
															   .fontFace = (std::move(fontFace)),
															   .cpuFontMap = std::move(fontMap),
															   .gpuFontMap = (std::move(gpuFontMap)),
															   .descriptorSet = queue.GetDevice().
																					  AllocateDescriptorSet(mPipelineLayout
																											.GetSetLayout(1))
														   }).first->second;

		insertedFontmap.descriptorSet.SetUniformBuffer(0, 0, mConstantsBuffer);
		insertedFontmap.descriptorSet.SetTexture(1, 0, insertedFontmap.gpuFontMap.mView, VK_IMAGE_LAYOUT_GENERAL);
		insertedFontmap.descriptorSet.SetSampler(2, 0, insertedFontmap.gpuFontMap.sampler, VK_IMAGE_LAYOUT_GENERAL);
	}
}
