#pragma once
// Strawberry UI
#include "Strawberry/UI/Rendering/Text/FontMap.hpp"
// Strawberry Vulkan
#include "Strawberry/Vulkan/Descriptor/DescriptorPool.hpp"
#include "Strawberry/Vulkan/Descriptor/DescriptorSet.hpp"
#include "Strawberry/Vulkan/Pipeline/GraphicsPipeline.hpp"
#include "Strawberry/Vulkan/Pipeline/PipelineLayout.hpp"
#include "Strawberry/Vulkan/Resource/Buffer.hpp"
// Strawberry Core
#include "Strawberry/Core/Math/Matrix.hpp"
#include "Strawberry/Vulkan/Queue/Batch.hpp"


namespace Strawberry::UI
{
	class FontMap;
	class TextNode;
	class ColoredNode;
	class Node;

	struct GlyphEntry
	{
		Core::Math::Vec2f position;
		Core::Math::Vec2f extent;
		unsigned int      glyphPageIndex;
		Core::Math::Vec2f glyphPosition;
		Core::Math::Vec2f glyphExtent;
	};


	class ColoredNodeRenderer
	{
	public:
		struct RenderQueueEntry
		{
			Core::Math::Vec2f position;
			Core::Math::Vec2f extent;
			Core::Math::Vec4f color;
		};


		ColoredNodeRenderer(Vulkan::Framebuffer& framebuffer, uint32_t subpassIndex, const Core::Math::Mat4f& projectionMatrix, Core::Math::Vec2f contentScale);


		Vulkan::Batch MakeBatch(const ColoredNode& node);


		void RenderBatch(const Vulkan::Buffer& batch) const;


	private:
		static Vulkan::PipelineLayout CreateColouredNodePipelineLayout(Vulkan::Device& device);

		static Vulkan::GraphicsPipeline CreateColouredNodePipeline(Vulkan::Framebuffer&    renderPass,
																   Vulkan::PipelineLayout& pipelineLayout,
																   uint32_t                subpassIndex);


		Vulkan::PipelineLayout   mColouredNodePipelineLayout;
		Vulkan::GraphicsPipeline mColouredNodePipeline;


		Core::Math::Mat4f      mProjectionMatrix;
		Core::Math::Vec2f      mContentScale;
		Vulkan::DescriptorPool mDescriptorPool;
		Vulkan::DescriptorSet  mRenderConstantsDescriptorSet;
		Vulkan::Buffer         mRenderConstantsBuffer;
	};
}
