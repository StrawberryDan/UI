#include "TextNodeRenderer.hpp"
#include "RenderContext.hpp"

#include "Strawberry/UI/TextNode.hpp"
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


	TextNodeRenderer::TextNodeRenderer(
		Vulkan::Framebuffer& framebuffer,
		unsigned int subpassIndex,
		const Core::Math::Mat4f projectionMatrix,
		Core::Math::Vec2f    contentScale)
		: mPipelineLayout([&]() {
			  return Vulkan::PipelineLayout::Builder(framebuffer.GetDevice())
					 .WithDescriptor(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
									 VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
					 .WithDescriptor(0, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
									 VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
									 1)
					 .WithDescriptor(0, VK_DESCRIPTOR_TYPE_SAMPLER,
									 VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
									 1)
					 .Build();
		  }())
		  , mGraphicsPipeline([&]() -> Vulkan::GraphicsPipeline {
			  return Vulkan::GraphicsPipeline::Builder(mPipelineLayout, framebuffer.GetRenderPass(), subpassIndex)
					 .WithInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
					 .WithInputBinding(0, VK_VERTEX_INPUT_RATE_INSTANCE)
					 .WithInputAttribute(0, 0, sizeof(RenderQueueEntry::position), VK_FORMAT_R32G32_SFLOAT)
					 .WithInputAttribute(1, 0, sizeof(RenderQueueEntry::extent), VK_FORMAT_R32G32_SFLOAT)
					 .WithInputAttribute(2, 0, sizeof(RenderQueueEntry::glyphAddressPageIndex), VK_FORMAT_R32_UINT)
					 .WithInputAttribute(3, 0, sizeof(RenderQueueEntry::glyphAddressCoordinate), VK_FORMAT_R32G32_UINT)
					 .WithInputAttribute(4, 0, sizeof(RenderQueueEntry::glyphAddressExtent), VK_FORMAT_R32G32_UINT)
					 .WithViewport(framebuffer, false)
					 .WithAlphaColorBlending()
					 .WithRasterization(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE)
					 .WithShaderStages(Vulkan::Shader::Compile(framebuffer.GetDevice(), VERTEX_SHADER).Unwrap(),
									   Vulkan::Shader::Compile(framebuffer.GetDevice(), FRAGMENT_SHADER).Unwrap())
					 .Build();
		  }())
		  , mGPUGlyphBuffer(Vulkan::Buffer::Builder(framebuffer.GetDevice(), Vulkan::MemoryTypeCriteria::HostVisible())
							.WithSize(sizeof(Glyph) * DEFAULT_GLYPH_BUFFER_SIZE)
							.WithUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
							.Build())
		  , mGlyphBufferDescriptorSet(Vulkan::DescriptorSet::Allocate(framebuffer.GetDevice(), mPipelineLayout.GetSetLayout(0)).Unwrap())
		  , mContentScale(contentScale)
		  , mConstantsBuffer(
				Vulkan::Buffer::Builder(
				framebuffer.GetDevice(),
				Vulkan::MemoryTypeCriteria::HostVisible())
				.WithData(Core::IO::DynamicByteBuffer::FromObjects(projectionMatrix, mPageSize))
				.WithUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
				.Build())
	{}


	Vulkan::Batch TextNodeRenderer::MakeBatch(const TextNode& node, const RenderContext& renderContext)
	{
		Vulkan::Batch batch(mGraphicsPipeline);
		batch.WithVertexCount(6);
		batch.WithInstanceCount((unsigned int) node.GetString().size());

		Core::IO::DynamicByteBuffer inputBufferData;

		LoadedFont& font = mFontMaps.at(node.GetFont());

		Core::Math::Vec2f glyphCursor;
		for (char32_t codepoint: node.GetString())
		{
			auto                  glyph        = font.fontFace.LoadGlyphOfCodepoint(codepoint).Unwrap();
			FontMap::GlyphAddress glyphAddress = font.cpuFontMap.GetGlyphAddress(codepoint).Unwrap();

			auto pos = renderContext.position + glyphCursor + glyph.Bearing();

			inputBufferData.Push(mContentScale.Piecewise(std::multiplies{}, pos));
			inputBufferData.Push(glyphAddress.extent.Piecewise(std::multiplies{}, mContentScale));
			inputBufferData.Push(glyphAddress.pageIndex);
			inputBufferData.Push(glyphAddress.offset);
			inputBufferData.Push(glyphAddress.extent);

			glyphCursor += glyph.Advance();
		}

		batch.WithVertexBuffer(0,
			Vulkan::Buffer::Builder(mGraphicsPipeline.GetDevice(), Vulkan::MemoryTypeCriteria::HostVisible())
			.WithData(inputBufferData)
			.WithUsage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
			.Build());

		batch.WithDescriptorSet(0, &mFontMaps.at(node.GetFont()).descriptorSet);

		return std::move(batch);
	}


	// void TextNodeRenderer::Render(Vulkan::CommandBuffer& commandBuffer, Core::Math::Mat4f projectionMatrix)
	// {
	// 	Core::IO::DynamicByteBuffer glyphBufferContents;
	//
	// 	if (!mGlyphBuffer.empty())
	// 	{
	// 		mConstantsBuffer.SetData(
	// 			Core::IO::DynamicByteBuffer::FromObjects(
	// 			projectionMatrix,
	// 			mFontMaps.at(mGlyphBuffer[0].font).
	// 				cpuFontMap.GetPageSize()));
	//
	// 		for (auto& glyph: mGlyphBuffer)
	// 		{
	// 			auto position = glyph.position;
	// 			position[0]   *= mContentScale[0];
	// 			position[1]   *= mContentScale[1];
	//
	// 			auto size = glyph.size;
	// 			size[0]   *= mContentScale[0];
	// 			size[1]   *= mContentScale[1];
	//
	// 			glyphBufferContents.Push(position);
	// 			glyphBufferContents.Push(size);
	// 			glyphBufferContents.Push(glyph.glyphAddressPageIndex);
	// 			glyphBufferContents.Push<uint32_t>(0);
	// 			glyphBufferContents.Push(glyph.glyphAddressCoordinate);
	// 		}
	//
	// 		mGPUGlyphBuffer.SetData(glyphBufferContents);
	//
	// 		commandBuffer.BindPipeline(mGraphicsPipeline);
	// 		commandBuffer.BindDescriptorSet(mGraphicsPipeline, 0, mGlyphBufferDescriptorSet);
	// 		commandBuffer.BindDescriptorSet(mGraphicsPipeline, 1,
	// 										mFontMaps.at(mGlyphBuffer.front().font).descriptorSet);
	// 		commandBuffer.Draw(6, static_cast<unsigned int>(mGlyphBuffer.size()));
	// 		mGlyphBuffer.clear();
	// 	}
	// }


	void TextNodeRenderer::LoadFont(Vulkan::Queue& queue, std::string ID, FontFace&& fontFace)
	{
		FontMap    fontMap(fontFace, FT_RENDER_MODE_NORMAL, mPageSize);
		GPUFontMap gpuFontMap = fontMap.CopyToGPU(queue);

		auto& insertedFontmap = mFontMaps.insert_or_assign(
														   ID, LoadedFont
														   {
															   .fontFace = (std::move(fontFace)),
															   .cpuFontMap = std::move(fontMap),
															   .gpuFontMap = (std::move(gpuFontMap)),
															   .descriptorSet = queue.GetDevice()
																					 .AllocateDescriptorSet(mPipelineLayout
																											.GetSetLayout(0)).Unwrap()
														   }).first->second;

		insertedFontmap.descriptorSet.SetUniformBuffer(0, 0, mConstantsBuffer);
		insertedFontmap.descriptorSet.SetTexture(1, 0, insertedFontmap.gpuFontMap.mView, VK_IMAGE_LAYOUT_GENERAL);
		insertedFontmap.descriptorSet.SetSampler(2, 0, insertedFontmap.gpuFontMap.sampler, VK_IMAGE_LAYOUT_GENERAL);
	}
}
