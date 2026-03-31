#pragma once
#include "Strawberry/Core/Math/Matrix.hpp"
#include "Strawberry/Vulkan/Descriptor/DescriptorPool.hpp"
#include "Strawberry/Vulkan/Descriptor/Sampler.hpp"
#include "Strawberry/Vulkan/Pipeline/GraphicsPipeline.hpp"
#include "Strawberry/Vulkan/Resource/Framebuffer.hpp"
#include "Strawberry/Vulkan/Queue/Batch.hpp"


namespace Strawberry::UI
{
	class SpriteNode;
	struct RenderContext;

	class SpriteNodeRenderer
	{
	public:
		SpriteNodeRenderer(Vulkan::Framebuffer& framebuffer, uint32_t subpassIndex, const Core::Math::Mat4f& projectionMatrix, const Core::Math::Vec2f& contentScale);


		Vulkan::Batch MakeBatch(const SpriteNode& spriteNode, const RenderContext& renderContext);


	private:
		Core::Math::Vec2f                                      mContentScale;
		Vulkan::PipelineLayout                                 mPipelineLayout;
		Vulkan::GraphicsPipeline                               mPipeline;
		Vulkan::DescriptorSet                                  mDrawConstantsDescriptor;
		Vulkan::Buffer                                         mDrawConstantsBuffer;
		Vulkan::Sampler                                        mImageSampler;
		std::map<Vulkan::Image::Handle, Vulkan::DescriptorSet> mTextureDescriptors;
		std::map<Vulkan::Image::Handle, Vulkan::ImageView>     mImageViews;
	};

	class SpriteNode;
}
