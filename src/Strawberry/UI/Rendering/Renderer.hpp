#pragma once
#include "ColoredNodeRenderer.hpp"
#include "TextNodeRenderer.hpp"
#include "Strawberry/Vulkan/Memory/ChainAllocator.hpp"


namespace Strawberry::UI
{
	class Renderer
	{
	public:
		Renderer(Vulkan::Framebuffer& framebuffer, size_t subpassIndex);


		void Submit(const Node& node);
		void SubmitColouredNode(const ColoredNode& node);
		void SubmitTextNode(const TextNode& node);


		void Render(Vulkan::CommandBuffer& commandBuffer);


		Vulkan::FallbackChainAllocator<Vulkan::FreeListAllocator>& GetBufferAllocator() noexcept { return mBufferAllocator; }
		Vulkan::FallbackChainAllocator<Vulkan::FreeListAllocator>& GetTextureAllocator() noexcept { return mTextureAllocator; }


		ColoredNodeRenderer& GetColoredNodeRenderer() noexcept { return mColoredNodeRenderer; }
		TextNodeRenderer& GetTextNodeRenderer() noexcept { return mTextNodeRenderer; }


	private:
		static Core::Math::Mat4f CreateProjectionMatrix(Vulkan::Framebuffer& framebuffer);


		unsigned int mLastDrawIndex = 0;
		Core::Math::Mat4f mProjectionMatrix;


		Vulkan::FallbackChainAllocator<Vulkan::FreeListAllocator> mBufferAllocator;
		Vulkan::FallbackChainAllocator<Vulkan::FreeListAllocator> mTextureAllocator;


		ColoredNodeRenderer mColoredNodeRenderer;
		TextNodeRenderer mTextNodeRenderer;
	};
}
