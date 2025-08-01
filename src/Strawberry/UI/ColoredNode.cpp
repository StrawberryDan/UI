#include "ColoredNode.hpp"

#include "Rendering/ColoredNodeRenderer.hpp"
#include "Rendering/Renderer.hpp"


namespace Strawberry::UI
{
	ColoredNode::ColoredNode(Position position, Extent extent, Color color)
		: Node(position, extent)
		, mColor(color)
	{}

	void ColoredNode::Render(Renderer& renderer) const
	{
		renderer.SubmitColouredNode(*this);
	}
}
