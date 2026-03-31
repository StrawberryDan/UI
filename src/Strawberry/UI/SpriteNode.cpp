#include "SpriteNode.hpp"


namespace Strawberry::UI
{
	SpriteNode::SpriteNode(TwoD::TextureReference&& sprite)
		: Node()
		, mTextureReference(std::move(sprite))
	{
		SetExtent(mTextureReference.Region().Size().AsType<float>());
	}


	void SpriteNode::SetScale(float scale)
	{
		mScale = scale;
	}


	const TwoD::TextureReference& SpriteNode::GetTexture() const
	{
		return mTextureReference;
	}


	void SpriteNode::SetSprite(const TwoD::TextureAtlas& atlas, const TwoD::TextureAtlas::Handle& handle)
	{
		mTextureReference = atlas.GetTextureReference(handle);
		SetExtent(mTextureReference.Region().Size().AsType<float>());
	}


	void SpriteNode::SetSprite(const TwoD::TextureAtlas& atlas, TwoD::TextureAtlas::Key key)
	{
		SetSprite(atlas.GetTextureReference(key));
	}


	void SpriteNode::SetSprite(TwoD::TextureReference&& sprite)
	{
		mTextureReference = std::move(sprite);
		SetExtent(mTextureReference.Region().Size().AsType<float>());
	}
}
