#pragma once
#include "Node.hpp"


namespace Strawberry::UI
{
	class ColoredNode
		: public Node
	{
	public:
		using Color = Core::Math::Vec4f;


		ColoredNode() = default;
		ColoredNode(Position position, Extent extent, Color color);


		Color GetColor()      const noexcept { return mColor; }
		Color SetColor(Color color) noexcept { return mColor = color; }


		void Render(NodeRenderer& nodeRenderer) const override;


	private:
		Color mColor;
	};
}
