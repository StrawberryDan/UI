#pragma once
#include "ColoredNodeRenderer.hpp"
#include "TextNodeRenderer.hpp"
#include "../../../../../Vulkan/src/Strawberry/Vulkan/Memory/Allocator/ChainAllocator.hpp"
#include "Strawberry/UI/NodeTree.hpp"


namespace Strawberry::UI
{
	class NodeTree;

	class Renderer
	{
	public:
		Renderer(Vulkan::Framebuffer& framebuffer, uint32_t subpassIndex, Core::Math::Vec2f contentScale);


		void Submit(const NodeTree& nodeTree);
		void Submit(const Node& node);
		void Submit(const ColoredNode& node);
		void Submit(const TextNode& node);


		void Render(Vulkan::CommandBuffer& commandBuffer);


		ColoredNodeRenderer& GetColoredNodeRenderer() noexcept { return mColoredNodeRenderer; }
		TextNodeRenderer& GetTextNodeRenderer() noexcept { return mTextNodeRenderer; }


	private:
		static Core::Math::Mat4f CreateProjectionMatrix(Vulkan::Framebuffer& framebuffer);


		unsigned int mLastDrawIndex = 0;
		Core::Math::Mat4f mProjectionMatrix;

		Core::Math::Vec2f mContentScale;
		ColoredNodeRenderer mColoredNodeRenderer;
		TextNodeRenderer mTextNodeRenderer;
	};
}
