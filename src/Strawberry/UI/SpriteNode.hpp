#pragma once


#include "Node.hpp"
#include "Strawberry/2D/TextureAtlas.hpp"
#include "Strawberry/2D/TextureReference.hpp"


namespace Strawberry::UI
{
	class SpriteNode
		: public Node
	{
	public:
		SpriteNode(TwoD::TextureReference&& sprite);

		float GetScale() const { return mScale; }
		void SetScale(float scale);

		const TwoD::TextureReference& GetTexture() const;

		void SetSprite(const TwoD::TextureAtlas& atlas, const TwoD::TextureAtlas::Handle& handle);
		void SetSprite(const TwoD::TextureAtlas& atlas, TwoD::TextureAtlas::Key key);
		void SetSprite(TwoD::TextureReference&& sprite);

	private:
		float mScale = 1.0f;
		TwoD::TextureReference mTextureReference;
	};
}
