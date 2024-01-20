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
		Align(VerticalAlignment::Center);
		Align(HorizontalAlignment::Center);
	}


	void RectangularNode::Align(HorizontalAlignment alignment, float padding)
	{
		Core::Assert(GetParent<RectangularNode>().HasValue());

		switch (alignment)
		{
			case HorizontalAlignment::Left:
				SetLocalPosition({padding, GetLocalPosition()[1]});
				break;
			case HorizontalAlignment::Center:
				SetLocalPosition({GetParent<RectangularNode>()->GetLocalSize()[0] / 2.0f - GetSize()[0] / 2.0f + padding, GetLocalPosition()[1]});
				break;
			case HorizontalAlignment::Right:
				SetLocalPosition({GetParent<RectangularNode>()->GetLocalSize()[0] - GetSize()[0] - padding, GetLocalPosition()[1]});
				break;
		}
	}


	void RectangularNode::Align(VerticalAlignment alignment, float padding)
	{
		Core::Assert(GetParent<RectangularNode>().HasValue());

		switch (alignment)
		{
			case VerticalAlignment::Top:
				SetLocalPosition({GetLocalPosition()[0], padding});
				break;
			case VerticalAlignment::Center:
				SetLocalPosition({GetLocalPosition()[0], GetParent<RectangularNode>()->GetLocalSize()[1] / 2.0f - GetSize()[1] / 2.0f + padding});
				break;
			case VerticalAlignment::Bottom:
				SetLocalPosition({GetLocalPosition()[0], GetParent<RectangularNode>()->GetLocalSize()[1] - GetSize()[1] - padding});
				break;
		}
	}


	void RectangularNode::Align(Anchor anchor, float padding)
	{
		switch (anchor)
		{
			case Anchor::North:
				Align(VerticalAlignment::Top, padding);
				Align(HorizontalAlignment::Center);
				break;
			case Anchor::NorthEast:
				Align(VerticalAlignment::Top, padding);
				Align(HorizontalAlignment::Right, padding);
				break;
			case Anchor::East:
				Align(VerticalAlignment::Center);
				Align(HorizontalAlignment::Right, padding);
				break;
			case Anchor::SouthEast:
				Align(VerticalAlignment::Bottom, padding);
				Align(HorizontalAlignment::Right, padding);
				break;
			case Anchor::South:
				Align(VerticalAlignment::Bottom, padding);
				Align(HorizontalAlignment::Center);
				break;
			case Anchor::SouthWest:
				Align(VerticalAlignment::Bottom, padding);
				Align(HorizontalAlignment::Left, padding);
				break;
			case Anchor::West:
				Align(VerticalAlignment::Center);
				Align(HorizontalAlignment::Left, padding);
				break;
			case Anchor::NorthWest:
				Align(VerticalAlignment::Top, padding);
				Align(HorizontalAlignment::Left, padding);
				break;
		}
	}


	void RectangularNode::AlignRelative(RectangularNode& node, VerticalAlignment alignment, float padding)
	{
		Core::Assert(GetParent() == node.GetParent());


		auto position = GetLocalPosition();
		switch (alignment)
		{
			case VerticalAlignment::Top:
				position[1] = node.GetLocalPosition()[1] - GetLocalSize()[1] - padding;
				break;
			case VerticalAlignment::Center:
				position[1] = node.GetLocalPosition()[1] + (node.GetLocalSize()[1] - GetLocalSize()[1]) / 2.0f + padding;
				break;
			case VerticalAlignment::Bottom:
				position[1] = node.GetLocalPosition()[1] + node.GetLocalSize()[1] + padding;
				break;
		}
		SetLocalPosition(position);
	}


	void RectangularNode::AlignRelative(RectangularNode& node, HorizontalAlignment alignment, float padding)
	{
		Core::Assert(GetParent() == node.GetParent());


		auto position = GetLocalPosition();
		switch (alignment)
		{
			case HorizontalAlignment::Left:
				position[0] = node.GetLocalPosition()[0] - GetLocalSize()[0] - padding;
				break;
			case HorizontalAlignment::Center:
				position[0] = node.GetLocalPosition()[0] + (node.GetLocalSize()[0] - GetLocalSize()[0]) / 2.0f + padding;
				break;
			case HorizontalAlignment::Right:
				position[0] = node.GetLocalPosition()[0] + node.GetLocalSize()[0] + padding;
				break;
		}
		SetLocalPosition(position);
	}


	void RectangularNode::PositionRelative(RectangularNode& node, VerticalAlignment position, HorizontalAlignment alignment, float padding)
	{
		Core::Assert(GetParent() == node.GetParent());
		Core::Math::Vec2f newPosition = node.GetLocalPosition();

		switch (position)
		{
			case VerticalAlignment::Top:
				newPosition[1] -= GetLocalSize()[1] - padding;
				break;
			case VerticalAlignment::Center:
				newPosition[1] += padding;
				break;
			case VerticalAlignment::Bottom:
				newPosition[1] += node.GetLocalSize()[1] + padding;
				break;
		}

		switch (alignment)
		{
			case HorizontalAlignment::Left:
				break;
			case HorizontalAlignment::Center:
				newPosition[0] += node.GetLocalSize()[0] / 2.0 - GetLocalSize()[0] / 2.0;
				break;
			case HorizontalAlignment::Right:
				newPosition[0] += node.GetLocalSize()[0] - GetLocalSize()[0];
				break;
		}

		SetLocalPosition(newPosition);
	}


	void RectangularNode::PositionRelative(RectangularNode& node, HorizontalAlignment position, VerticalAlignment alignment, float padding)
	{
		Core::Assert(GetParent() == node.GetParent());
		Core::Math::Vec2f newPosition = node.GetLocalPosition();

		switch (position)
		{
			case HorizontalAlignment::Left:
				newPosition[0] -= GetLocalSize()[0] - padding;
			break;
			case HorizontalAlignment::Center:
				newPosition[0] += padding;
			break;
			case HorizontalAlignment::Right:
				newPosition[0] += node.GetLocalSize()[0] + padding;
			break;
		}

		switch (alignment)
		{
			case VerticalAlignment::Top:
				break;
			case VerticalAlignment::Center:
				newPosition[1] += node.GetLocalSize()[1] / 2.0 - GetLocalSize()[1] / 2.0;
			break;
			case VerticalAlignment::Bottom:
				newPosition[1] += node.GetLocalPosition()[1] - GetLocalSize()[1];
			break;
		}

		SetLocalPosition(newPosition);
	}


	void RectangularNode::PositionRelative(RectangularNode& node, Anchor anchor, float padding)
	{
		Core::Assert(node.GetParent() == GetParent());


		Core::Math::Vec2f offset;
		switch (anchor)
		{
			case Anchor::North:
				offset = {node.GetLocalSize()[0] / 2.0f - GetLocalSize()[0] / 2.0f, -GetLocalSize()[1] - padding};
				break;
			case Anchor::NorthEast:
				offset = {node.GetLocalSize()[0] + padding, -GetLocalSize()[1] - padding};
				break;
			case Anchor::East:
				offset = {node.GetLocalSize()[0] + padding, node.GetLocalSize()[1] / 2.0f - GetLocalSize()[1] / 2.0f};
				break;
			case Anchor::SouthEast:
				offset = {node.GetLocalSize()[0] + padding, node.GetLocalSize()[1] + padding};
				break;
			case Anchor::South:
				offset = {node.GetLocalSize()[0] / 2.0f - GetLocalSize()[0] / 2.0f, node.GetLocalSize()[1] + padding};
				break;
			case Anchor::SouthWest:
				offset = {-GetLocalSize()[0] - padding, node.GetLocalSize()[1] + padding};
				break;
			case Anchor::West:
				offset = {-GetLocalSize()[0] - padding, node.GetLocalSize()[1] / 2.0f - GetLocalSize()[1] / 2.0f};
				break;
			case Anchor::NorthWest:
				offset = {-GetLocalSize()[0] - padding, -GetLocalSize()[1] - padding};
				break;
		}


		SetLocalPosition(node.GetLocalPosition() + offset);
	}


	void RectangularNode::Inset(float amount)
	{
		SetLocalSize(GetParent<RectangularNode>()->GetLocalSize() - Core::Math::Vec2f(amount, amount));
		Center();
	}
}
