#include "ColoredNode.hpp"

#include "Rendering/NodeRenderer.hpp"


namespace Strawberry::UI
{
	ColoredNode::ColoredNode(Position position, Extent extent, Color color)
		: Node(position, extent)
		, mColor(color)
	{}

	void ColoredNode::Render(NodeRenderer& nodeRenderer) const
	{
		nodeRenderer.SubmitColouredNode(*this);
	}
}
