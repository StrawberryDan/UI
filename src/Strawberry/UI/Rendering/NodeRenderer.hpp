#pragma once
#include "Strawberry/Core/Math/Matrix.hpp"
#include "Strawberry/Vulkan/GraphicsPipeline.hpp"
#include "Strawberry/Vulkan/PipelineLayout.hpp"
#include "Strawberry/Vulkan/Framebuffer.hpp"
#include "Strawberry/Vulkan/Memory/FreelistAllocator.hpp"


namespace Strawberry::UI
{
	class ColoredNode;
	class Node;

	class NodeRenderer
	{
	public:
		NodeRenderer(Vulkan::Framebuffer& framebuffer);


		void Submit(const Node& node);

		void SubmitColouredNode(const ColoredNode& node);

		void Render(Vulkan::CommandBuffer& commandBuffer);


	private:
		static Core::Math::Mat4f CreateProjectionMatrix(Vulkan::Framebuffer& framebuffer);
		static Vulkan::PipelineLayout CreatePipelineLayout(Vulkan::Device& device);
		static Vulkan::GraphicsPipeline CreatePipeline(Vulkan::Framebuffer& renderPass, Vulkan::PipelineLayout& pipelineLayout);


		struct Entry
		{
			Core::Math::Vec2f position;
			Core::Math::Vec2f extent;
			Core::Math::Vec4f color;
		};


		std::vector<Entry> mEntries;


		Core::Math::Mat4f mProjectionMatrix;


		Vulkan::PipelineLayout mPipelineLayout;
		Vulkan::GraphicsPipeline mPipeline;


		Vulkan::FreeListAllocator mAllocator;
		Vulkan::Buffer mInputBuffer;

		Vulkan::DescriptorPool mDescriptorPool;
		Vulkan::DescriptorSet mRenderConstantsDescriptorSet;
		Vulkan::Buffer mRenderConstantsBuffer;
	};
}
