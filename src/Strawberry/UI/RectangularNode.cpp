//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "RectangularNode.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	void RectangularNode::Update(Core::Seconds deltaTime)
	{
		Node::Update(deltaTime);
	}


	Core::Math::Vec2f RectangularNode::GetSize() const
	{
		return GetScale() * GetLocalSize();
	}


	Core::Math::Vec2f RectangularNode::GetLocalSize() const
	{
		return mLocalSize;
	}


	void RectangularNode::SetLocalSize(Core::Math::Vec2f size)
	{
		mLocalSize = size;
	}


	bool RectangularNode::ContainsPoint(Core::Math::Vec2f screenPosition)
	{
		auto relative = screenPosition - GetPosition();
		const Core::Math::Vec2f size = GetSize();
		return relative[0] >= 0.0f && relative[0] <= size[0] && relative[1] >= 0.0f && relative[1] <= size[1];
	}


	void RectangularNode::Center()
	{
		Core::Assert(GetParent<RectangularNode>().HasValue());
		SetLocalPosition(GetParent<RectangularNode>()->GetSize() / 2.0f - GetSize() / 2.0f);
	}


	void RectangularNode::CenterHorizontally()
	{
		Core::Assert(GetParent<RectangularNode>().HasValue());
		SetLocalPosition({GetParent<RectangularNode>()->GetSize()[0] / 2.0f - GetSize()[0] / 2.0f, GetLocalPosition()[1]});
	}


	void RectangularNode::CenterVertically()
	{
		Core::Assert(GetParent<RectangularNode>().HasValue());
		SetLocalPosition({GetLocalPosition()[0], GetParent<RectangularNode>()->GetSize()[1] / 2.0f - GetSize()[1] / 2.0f});
	}


	void RectangularNode::PositionRelative(RectangularNode& node, RelativePosition position, float padding)
	{
		Core::Assert(GetParent() == node.GetParent());

		switch (position)
		{
			case RelativePosition::Above:
				SetLocalPosition({node.GetLocalPosition()[0], node.GetLocalPosition()[1] - GetSize()[1] - padding});
				break;
			case RelativePosition::Left:
				SetLocalPosition({node.GetLocalPosition()[0] - GetSize()[0] - padding, node.GetLocalPosition()[1]});
				break;
			case RelativePosition::Right:
				SetLocalPosition({node.GetLocalPosition()[0] + node.GetSize()[0] + padding, node.GetLocalPosition()[1]});
				break;
			case RelativePosition::Below:
				SetLocalPosition({node.GetLocalPosition()[0], node.GetLocalPosition()[1] + node.GetSize()[1] + padding});
				break;
		}
	}
}
