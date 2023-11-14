#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Node.hpp"
// Strawberry Graphics
#include "Strawberry/Graphics/2D/Sprite.hpp"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class Sprite
		: public Node
	{
	public:
		Sprite(Graphics::Sprite sprite);
		Sprite(Graphics::SpriteSheet& spriteSheet);


		void Render(Renderer& renderer) override;


		bool ContainsPoint(Core::Math::Vec2f screenPosition) override;


		Graphics::Sprite& GetSprite();
		const Graphics::Sprite& GetSprite() const;
		void SetSprite(Graphics::Sprite sprite);


	private:
		Graphics::Sprite mSprite;
	};
}