#include "Node.hpp"


namespace Strawberry::UI
{
	Node::Node()
		: mParent(nullptr)
	{}


	Node::Node(Node& node)
		: mParent(node)
	{}


	Core::Math::Vec2f Node::GetPosition() const
	{
		return mParent ? mParent->GetPosition() : Core::Math::Vec2f() + GetLocalPosition();
	}


	Core::Math::Vec2f Node::GetLocalPosition() const
	{
		return mLocalPosition;
	}


	Core::Math::Vec2f Node::GetSize() const
	{
		return GetScale() * GetLocalSize();
	}


	Core::Math::Vec2f Node::GetLocalSize() const
	{
		return mLocalSize;
	}


	Core::Math::Vec2f Node::GetScale() const
	{
		return mParent ? mParent->GetScale() : Core::Math::Vec2f(1.0f, 1.0f) * GetLocalScale();
	}


	Core::Math::Vec2f Node::GetLocalScale() const
	{
		return mLocalScale;
	}


	Core::ReflexivePointer<Node> Node::GetChild(size_t index)
	{
		return mChildren[index]->GetReflexivePointer();
	}
}