#include "Renderer.hpp"

#include "Strawberry/UI/Rendering/RenderContext.hpp"
#include "Strawberry/Vulkan/Resource/Framebuffer.hpp"
#include "Strawberry/Core/Math/Transformations.hpp"
#include "Strawberry/UI/ColoredNode.hpp"
#include "Strawberry/UI/TextNode.hpp"
#include "Strawberry/UI/Node.hpp"
#include "Strawberry/UI/NodeTree.hpp"
#include "Strawberry/UI/SpriteNode.hpp"


namespace Strawberry::UI
{
	Renderer::Renderer(Vulkan::Queue& queue, Vulkan::Framebuffer& framebuffer, uint32_t subpassIndex, Core::Math::Vec2f contentScale)
		: mProjectionMatrix(CreateProjectionMatrix(framebuffer))
		, mContentScale(contentScale)
		, mColoredNodeRenderer(framebuffer, subpassIndex, mProjectionMatrix, mContentScale)
		, mTextNodeRenderer(framebuffer, subpassIndex, mProjectionMatrix, mContentScale)
		, mSpriteNodeRenderer(framebuffer, subpassIndex, mProjectionMatrix, mContentScale)
		, mTextureAtlas(queue, {4096, 4096}, 1)
	{}


	void Renderer::Render(const NodeTree& nodeTree, Vulkan::CommandBuffer& commandBuffer)
	{
		mRenderBatcher.NewBatch();

		std::map<NodeTree::Config::NodeID, RenderContext> renderContextMap;

		nodeTree.Visit([&, this] (const NodeTree::VisitContext& visitContext)
		{
			auto parent = visitContext.GetParent();
			const auto parentContext = parent.HasValue() ? renderContextMap.at(parent.Value()) : RenderContext{};
			RenderContext myContext = parentContext.Apply(*visitContext.Value().get());
			renderContextMap.emplace(visitContext.CurrentNode(), myContext);

			if (auto coloredNode = dynamic_cast<const ColoredNode*>(visitContext.Value().get()))
			{
				mRenderBatcher.Enqueue(mColoredNodeRenderer.MakeBatch(*coloredNode, myContext));
			}
			else if (auto textNode = dynamic_cast<const TextNode*>(visitContext.Value().get()))
			{
				mRenderBatcher.Enqueue(mTextNodeRenderer.MakeBatch(*textNode, myContext));
			}
			else if (auto spriteNode = dynamic_cast<const SpriteNode*>(visitContext.Value().get()))
			{
				mRenderBatcher.Enqueue(mSpriteNodeRenderer.MakeBatch(*spriteNode, myContext));
			}
		});

		mRenderBatcher.WriteQueue(commandBuffer);
	}


	TwoD::TextureReference Renderer::GetTexture(const TwoD::TextureAtlas::Handle& handle)
	{
		return mTextureAtlas.GetTextureReference(handle);
	}


	Core::Math::Mat4f Renderer::CreateProjectionMatrix(Vulkan::Framebuffer& framebuffer)
	{
		return
			Core::Math::Translate<float>(-1.0f, -1.0f, 0.0f) *
			Core::Math::Scale<float>(1.0f / framebuffer.GetSize()[0], 1.0f / framebuffer.GetSize()[1], 1.0f);
	}
}
