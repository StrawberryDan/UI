//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Sprite.hpp"
#include "Rendering/Renderer.hpp"
// Strawberry Graphics
#include "Strawberry/Graphics/2D/SpriteSheet.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	Sprite::Sprite(Graphics::Sprite sprite)
		: mSprite(std::move(sprite))
	{
		SetLocalSize(mSprite.GetSpriteSheet()->GetSpriteSize().AsType<float>());
	}


	Sprite::Sprite(Graphics::SpriteSheet& spriteSheet)
		: mSprite(spriteSheet)
	{
		SetLocalSize(spriteSheet.GetSpriteSize().AsType<float>());
	}


	void Sprite::Render(Renderer& renderer)
	{
		if (IsVisible())
		{
			renderer.Render(*this);

			for (int i = 0; i < GetChildCount(); i++)
				GetChild(i)->Render(renderer);
		}
	}


	Graphics::Sprite& Sprite::GetSprite()
	{
		return mSprite;
	}


	const Graphics::Sprite& Sprite::GetSprite() const
	{
		return mSprite;
	}


	void Sprite::SetSprite(Graphics::Sprite sprite)
	{
		mSprite = std::move(sprite);
	}


	bool Sprite::ContainsPoint(Core::Math::Vec2f screenPosition)
	{
		if (!Node::ContainsPoint(screenPosition)) return false;


		Core::Math::Vec2f relative = screenPosition - GetPosition();
		if (relative[0] < 0.0 || relative[1] < 0.0) return false;


		Core::AssertEQ(mSprite.GetSpriteSheet()->GetImage().GetFormat(), VK_FORMAT_R8G8B8A8_SRGB);
		Core::Math::Vec2u pixel = relative
				.Map([](float x) { return std::round(x); }).AsType<unsigned int>();
		pixel = pixel + mSprite.GetSpriteCoords() * mSprite.GetSpriteSheet()->GetSpriteSize();
		auto pixelBytes = mSprite.GetSpriteSheet()->GetImage().ReadPixel(pixel);


		return pixelBytes[0] > 0 && pixelBytes[1] > 0 && pixelBytes[2] > 0 && pixelBytes[3] > 0;
	}
}