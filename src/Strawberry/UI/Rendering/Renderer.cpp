#include "Renderer.hpp"

#include "Strawberry/Core/Math/Transformations.hpp"
#include "Strawberry/UI/Node.hpp"


namespace Strawberry::UI
{
	Renderer::Renderer(Vulkan::Framebuffer& framebuffer, size_t subpassIndex)
		: mProjectionMatrix(CreateProjectionMatrix(framebuffer))
		, mBufferAllocator(Vulkan::ChainAllocator<Vulkan::FreeListAllocator>(
			*framebuffer.GetDevice(),
			framebuffer.GetDevice()->GetPhysicalDevice().SearchMemoryTypes(Vulkan::MemoryTypeCriteria::HostVisible())[0].index,
			1024 * 1024 * 64))
		, mTextureAllocator(Vulkan::ChainAllocator<Vulkan::FreeListAllocator>(
			*framebuffer.GetDevice(),
			framebuffer.GetDevice()->GetPhysicalDevice().SearchMemoryTypes(Vulkan::MemoryTypeCriteria::DeviceLocal())[0].index,
			1024 * 1024 * 64))
		, mColoredNodeRenderer(framebuffer, subpassIndex, mBufferAllocator)
		, mTextNodeRenderer()
	{
	}

	void Renderer::SubmitColouredNode(const ColoredNode& node)
	{
		mColoredNodeRenderer.Submit(mLastDrawIndex++, node);
	}

	void Renderer::SubmitTextNode(const TextNode& node)
	{
		mTextNodeRenderer.Submit(mLastDrawIndex++, node);
	}

	void Renderer::Render(Vulkan::CommandBuffer& commandBuffer)
	{
		mColoredNodeRenderer.Render(commandBuffer, mProjectionMatrix);
		mTextNodeRenderer.Render(commandBuffer, mProjectionMatrix);
	}

	void Renderer::Submit(const Node& node)
	{
		node.Render(*this);
	}


	Core::Math::Mat4f Renderer::CreateProjectionMatrix(Vulkan::Framebuffer& framebuffer)
	{
		return
			Core::Math::Translate<float>(-1.0f, -1.0f, 0.0f) *
			Core::Math::Scale<float>(1.0f / framebuffer.GetSize()[0], 1.0f / framebuffer.GetSize()[1], 1.0f);
	}
}
