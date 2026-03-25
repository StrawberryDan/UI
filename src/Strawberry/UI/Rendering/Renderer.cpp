#include "Renderer.hpp"

#include "Strawberry/Vulkan/Resource/Framebuffer.hpp"
#include "Strawberry/Core/Math/Transformations.hpp"
#include "Strawberry/UI/ColoredNode.hpp"
#include "Strawberry/UI/TextNode.hpp"
#include "Strawberry/UI/Node.hpp"
#include "Strawberry/UI/NodeTree.hpp"


namespace Strawberry::UI
{
	Renderer::Renderer(Vulkan::Framebuffer& framebuffer, uint32_t subpassIndex, Core::Math::Vec2f contentScale)
		: mProjectionMatrix(CreateProjectionMatrix(framebuffer))
		, mContentScale(contentScale)
		, mColoredNodeRenderer(framebuffer, subpassIndex, mProjectionMatrix, mContentScale)
		, mTextNodeRenderer(framebuffer, subpassIndex, mProjectionMatrix, mContentScale) {}


	void Renderer::Render(const NodeTree& nodeTree, Vulkan::CommandBuffer& commandBuffer)
	{
		mRenderBatcher.Clear();

		nodeTree.Visit([&, this] (const auto& x)
		{
			if (auto node = dynamic_cast<const ColoredNode*>(x.get()))
			{
				mRenderBatcher.Append(mColoredNodeRenderer.MakeBatch(*node));
			}
			else if (auto node = dynamic_cast<const TextNode*>(x.get()))
			{
				mRenderBatcher.Append(mTextNodeRenderer.MakeBatch(*node));
			}
		});

		mRenderBatcher.Render(commandBuffer);
	}


	Core::Math::Mat4f Renderer::CreateProjectionMatrix(Vulkan::Framebuffer& framebuffer)
	{
		return
			Core::Math::Translate<float>(-1.0f, -1.0f, 0.0f) *
			Core::Math::Scale<float>(1.0f / framebuffer.GetSize()[0], 1.0f / framebuffer.GetSize()[1], 1.0f);
	}
}
