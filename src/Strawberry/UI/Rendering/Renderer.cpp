#include "Renderer.hpp"

#include "../../../../../Vulkan/src/Strawberry/Vulkan/Resource/Framebuffer.hpp"
#include "Strawberry/Core/Math/Transformations.hpp"
#include "Strawberry/UI/Node.hpp"


namespace Strawberry::UI
{
	Renderer::Renderer(Vulkan::Framebuffer& framebuffer, uint32_t subpassIndex)
		: mProjectionMatrix(CreateProjectionMatrix(framebuffer))
		  , mColoredNodeRenderer(framebuffer, subpassIndex) {}


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
