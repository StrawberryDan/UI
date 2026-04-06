#include "SpriteNodeRenderer.hpp"

#include "RenderContext.hpp"
#include "Strawberry/Core/Math/Matrix.hpp"
#include "Strawberry/UI/SpriteNode.hpp"
#include "Strawberry/Vulkan/Resource/Buffer.hpp"


static const uint8_t VERTEX_SHADER[]
{
#include "SpriteNodeRenderer.vert.bin"
};


static const uint8_t FRAGMENT_SHADER[]
{
#include "SpriteNodeRenderer.frag.bin"
};


namespace Strawberry::UI
{
	SpriteNodeRenderer::SpriteNodeRenderer(Vulkan::Framebuffer& framebuffer, uint32_t subpassIndex, const Core::Math::Mat4f& projectionMatrix, const Core::Math::Vec2f& contentScale)
		: mContentScale(contentScale)
		, mPipelineLayout(
			Vulkan::PipelineLayout::Builder(framebuffer.GetDevice())
				.WithDescriptor(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
				.WithDescriptor(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
				.Build())
		, mPipeline(
			Vulkan::GraphicsPipeline::Builder(mPipelineLayout, framebuffer.GetRenderPass(), subpassIndex)
				.WithShaderStages(
					Vulkan::Shader::Compile(
						framebuffer.GetDevice(),
						VERTEX_SHADER).Unwrap(),
					Vulkan::Shader::Compile(
						framebuffer.GetDevice(),
						FRAGMENT_SHADER).Unwrap())
				.WithInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
				.WithInputBinding(0, VK_VERTEX_INPUT_RATE_INSTANCE)
				.WithInputAttribute(0, 0, sizeof(Core::Math::Vec2f), VK_FORMAT_R32G32_SFLOAT)
				.WithInputAttribute(1, 0, sizeof(Core::Math::Vec2f), VK_FORMAT_R32G32_SFLOAT)
				.WithInputAttribute(2, 0, sizeof(Core::Math::Vec2f), VK_FORMAT_R32G32_SFLOAT)
				.WithInputAttribute(3, 0, sizeof(Core::Math::Vec2f), VK_FORMAT_R32G32_SFLOAT)
				.WithInputAttribute(4, 0, sizeof(Core::Math::Vec2f), VK_FORMAT_R32_UINT)
				.WithViewport(framebuffer)
				.WithAlphaColorBlending()
				.WithRasterization(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE)
				.Build())
		, mDrawConstantsDescriptor(mPipeline.CreateDescriptorSet(0))
		, mDrawConstantsBuffer(Vulkan::Buffer::Builder(mPipeline.GetDevice(), Vulkan::MemoryTypeCriteria::HostVisible())
			.WithSize(sizeof(Core::Math::Mat4f))
			.WithUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
			.Build())
		, mImageSampler(framebuffer.GetDevice(), VK_FILTER_NEAREST, VK_FILTER_NEAREST)
	{
		mDrawConstantsBuffer.SetData(Core::IO::DynamicByteBuffer::FromObjects(projectionMatrix));
		mDrawConstantsDescriptor.SetUniformBuffer(0, 0, mDrawConstantsBuffer);
	}


	Vulkan::Batch SpriteNodeRenderer::MakeBatch(const SpriteNode& spriteNode, const RenderContext& renderContext)
	{
		if (!mTextureDescriptors.contains(spriteNode.GetTexture().Image()->GetHandle())) [[unlikely]]
		{
			auto imageView = Vulkan::ImageView::Builder(*spriteNode.GetTexture().Image(), VK_IMAGE_ASPECT_COLOR_BIT)
				.WithFormat(VK_FORMAT_R8G8B8A8_SRGB)
				.WithType(VK_IMAGE_VIEW_TYPE_2D_ARRAY)
				.Build();
			mImageViews.emplace(spriteNode.GetTexture().Image()->GetHandle(), std::move(imageView));


			Vulkan::DescriptorSet descriptorSet = mPipeline.CreateDescriptorSet(1);
			descriptorSet.SetCombinedImageSampler(0, 0, mImageSampler, mImageViews.at(spriteNode.GetTexture().Image()->GetHandle()), VK_IMAGE_LAYOUT_GENERAL);
			mTextureDescriptors.emplace(spriteNode.GetTexture().Image()->GetHandle(), std::move(descriptorSet));
		}

		Core::IO::DynamicByteBuffer inputBufferData;
		inputBufferData.Push(renderContext.position.Piecewise(std::multiplies{}, mContentScale));
		inputBufferData.Push(spriteNode.GetScale() * spriteNode.GetExtent().Piecewise(std::multiplies{}, mContentScale));
		inputBufferData.Push(spriteNode.GetTexture().Region().Min().AsType<float>().Piecewise(std::divides{}, spriteNode.GetTexture().Image()->GetSize().AsSize<2>()));
		inputBufferData.Push(spriteNode.GetTexture().Region().Max().AsType<float>().Piecewise(std::divides{}, spriteNode.GetTexture().Image()->GetSize().AsSize<2>()));
		inputBufferData.Push<uint32_t>(spriteNode.GetTexture().ArrayIndex());

		Vulkan::Buffer inputBuffer = Vulkan::Buffer::Builder(mPipeline.GetDevice(), Vulkan::MemoryTypeCriteria::HostVisible())
			.WithData(inputBufferData)
			.WithUsage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
			.Build();

		return std::move(Vulkan::Batch(mPipeline)
			.WithVertexBuffer(0, std::move(inputBuffer))
			.WithDescriptorSet(0, mDrawConstantsDescriptor)
			.WithDescriptorSet(1, mTextureDescriptors.at(spriteNode.GetTexture().Image()->GetHandle()))
			.WithVertexCount(6));
	}
}
