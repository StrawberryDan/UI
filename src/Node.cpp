#include "Node.hpp"


namespace Strawberry::UI
{
	Node::Node()
		: mParent(nullptr)
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


	Core::ReflexivePointer<Node> Node::GetParent() const
	{
		return Core::ReflexivePointer<Node>();
	}


	size_t Node::GetChildCount() const
	{
		return mChildren.size();
	}


	Core::ReflexivePointer<Node> Node::GetChild(size_t index)
	{
		return mChildren[index]->GetReflexivePointer();
	}


	Core::ReflexivePointer<Node> Node::AppendChild(std::unique_ptr<Node> node)
	{
		Core::Assert(node->GetParent() == nullptr);
		mChildren.emplace_back(std::move(node));
		return mChildren.back()->GetReflexivePointer();
	}


	Core::ReflexivePointer<Node> Node::PrependChild(std::unique_ptr<Node> node)
	{
		Core::Assert(node->GetParent() == nullptr);
		mChildren.emplace(mChildren.begin(), std::move(node));
		return mChildren.front()->GetReflexivePointer();
	}


	Core::ReflexivePointer<Node> Node::InsertChild(size_t index, std::unique_ptr<Node> node)
	{
		Core::Assert(node->GetParent() == nullptr);
		Core::Assert(index <= GetChildCount());
		mChildren.emplace(mChildren.begin() + static_cast<int>(index), std::move(node));
		return mChildren[index]->GetReflexivePointer();
	}
}