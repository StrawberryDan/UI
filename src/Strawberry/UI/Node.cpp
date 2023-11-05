#include "Node.hpp"
#include <Strawberry/Core/Assert.hpp>

namespace Strawberry::UI
{
	Node::Node()
		: mParent()
	{}


	Core::Math::Vec2f Node::GetPosition() const
	{
		auto lockedParent = mParent.lock();
		auto parentPosition = lockedParent ? lockedParent->GetPosition() : Core::Math::Vec2f();
		return parentPosition + GetLocalPosition();
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
		auto lockedParent = mParent.lock();
		auto parentScale = lockedParent ? lockedParent->GetScale() : Core::Math::Vec2f(1.0f, 1.0f);
		return parentScale * GetLocalScale();
	}


	Core::Math::Vec2f Node::GetLocalScale() const
	{
		return mLocalScale;
	}


	void Node::SetLocalPosition(Core::Math::Vec2f position)
	{
		mLocalPosition = position;
	}


	void Node::SetLocalSize(Core::Math::Vec2f size)
	{
		mLocalSize = size;
	}


	void Node::SetLocalScale(Core::Math::Vec2f scale)
	{
		mLocalScale = scale;
	}


	std::shared_ptr<Node> Node::GetParent() const
	{
		return mParent.lock();
	}


	size_t Node::GetChildCount() const
	{
		return mChildren.size();
	}


	std::shared_ptr<Node> Node::GetChild(size_t index)
	{
		return mChildren[index];
	}


	std::shared_ptr<Node> Node::AppendChild(std::shared_ptr<Node> node)
	{
		Core::Assert(node->GetParent() == nullptr);
		node->mParent = weak_from_this();
		mChildren.emplace_back(std::move(node));
		return mChildren.back();
	}


	std::shared_ptr<Node> Node::PrependChild(std::shared_ptr<Node> node)
	{
		Core::Assert(node->GetParent() == nullptr);
		node->mParent = shared_from_this();
		mChildren.emplace(mChildren.begin(), std::move(node));
		return mChildren.front();
	}


	std::shared_ptr<Node> Node::InsertChild(size_t index, std::shared_ptr<Node> node)
	{
		Core::Assert(node->GetParent() == nullptr);
		Core::Assert(index <= GetChildCount());
		node->mParent = shared_from_this();
		mChildren.emplace(mChildren.begin() + static_cast<int>(index), std::move(node));
		return mChildren[index];
	}
}
