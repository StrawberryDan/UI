#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Node.hpp"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	enum class HorizontalAlignment
	{
		Left,
		Center,
		Right,
	};


	enum class VerticalAlignment
	{
		Top,
		Center,
		Bottom,
	};


	enum class Anchor
	{
		North,
		NorthEast,
		East,
		SouthEast,
		South,
		SouthWest,
		West,
		NorthWest,
	};


	class RectangularNode
			: public Node
	{
	public:
		void Update(Core::Seconds deltaTime) override;


		[[nodiscard]] Core::Math::Vec2f GetSize() const;
		[[nodiscard]] Core::Math::Vec2f GetLocalSize() const;


		void SetLocalSize(Core::Math::Vec2f size);


		virtual bool ContainsPoint(Core::Math::Vec2f screenPosition);


		void Center();
		void Align(HorizontalAlignment alignment, float padding = 0.0f);
		void Align(VerticalAlignment alignment, float padding = 0.0f);
		void Align(Anchor anchor, float padding = 0.0f);


		void PositionRelative(RectangularNode& node, Anchor anchor, float padding = 0.0f);
		void PositionRelative(RectangularNode& node, VerticalAlignment position, HorizontalAlignment alignment = HorizontalAlignment::Center, float padding = 0.0f);
		void PositionRelative(RectangularNode& node, HorizontalAlignment position, VerticalAlignment alignment = VerticalAlignment::Center, float padding = 0.0f);


	private:
		Core::Math::Vec2f mLocalSize = Core::Math::Vec2f(0.0f, 0.0f);
	};
}
