#pragma once
#include "ColoredNodeRenderer.hpp"
#include "SpriteNodeRenderer.hpp"
#include "TextNodeRenderer.hpp"
#include "../../../../../Vulkan/src/Strawberry/Vulkan/Memory/Allocator/ChainAllocator.hpp"
#include "Strawberry/2D/TextureAtlas.hpp"
#include "Strawberry/UI/NodeTree.hpp"
#include "Strawberry/Vulkan/Queue/BatchRenderer.hpp"


namespace Strawberry::UI
{
	class NodeTree;


	class Renderer
	{
	private:
		struct Context;

	public:
		Renderer(Vulkan::Queue& queue, Vulkan::Framebuffer& framebuffer, uint32_t subpassIndex, Core::Math::Vec2f contentScale);

		void Render(const NodeTree& tree, Vulkan::CommandBuffer& commandBuffer);

		void LoadFont(Vulkan::Queue& queue, std::string ID, FontFace&& fontFace)
		{
			mTextNodeRenderer.LoadFont(queue, ID, std::move(fontFace));
		}

		void LoadTextures(const TwoD::TextureManifest& textureManifest)
		{
			mTextureAtlas.Register(textureManifest);
		}


		TwoD::TextureReference GetTexture(const TwoD::TextureAtlas::Handle& handle);


	private:


		static Core::Math::Mat4f CreateProjectionMatrix(Vulkan::Framebuffer& framebuffer);

		Core::Math::Vec2f mContentScale;
		Core::Math::Mat4f mProjectionMatrix;


		Vulkan::BatchRenderer mRenderBatcher;


		ColoredNodeRenderer mColoredNodeRenderer;
		TextNodeRenderer    mTextNodeRenderer;
		SpriteNodeRenderer  mSpriteNodeRenderer;


		TwoD::DiffuseTextureAtlas mTextureAtlas;
	};
}
