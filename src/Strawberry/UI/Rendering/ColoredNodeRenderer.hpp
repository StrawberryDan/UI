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
		ColoredNodeRenderer(Vulkan::Framebuffer& framebuffer, uint32_t subpassIndex);


		void Submit(uint32_t drawIndex, const ColoredNode& node);

		void Render(Vulkan::CommandBuffer& commandBuffer, Core::Math::Mat4f projectionMatrix);

	private:
		static Vulkan::PipelineLayout CreateColouredNodePipelineLayout(Vulkan::Device& device);

		static Vulkan::GraphicsPipeline CreateColouredNodePipeline(Vulkan::Framebuffer&    renderPass,
																   Vulkan::PipelineLayout& pipelineLayout,
																   uint32_t                subpassIndex);


		struct ColouredNodeEntry
		{
			unsigned int      drawIndex;
			Core::Math::Vec2f position;
			Core::Math::Vec2f extent;
			Core::Math::Vec4f color;
		};


		std::vector<ColouredNodeEntry> mEntries;


		Vulkan::PipelineLayout   mColouredNodePipelineLayout;
		Vulkan::GraphicsPipeline mColouredNodePipeline;


		Vulkan::Buffer mInputBuffer;

		Vulkan::DescriptorPool mDescriptorPool;
		Vulkan::DescriptorSet  mRenderConstantsDescriptorSet;
		Vulkan::Buffer         mRenderConstantsBuffer;
	};
}
