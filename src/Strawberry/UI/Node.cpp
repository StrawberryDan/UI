#include "Node.hpp"
#include "EventListener.hpp"
#include <Strawberry/Core/Assert.hpp>

namespace Strawberry::UI
{
	Node::Node()
		: mParent()
	{}


	void Node::Render(Renderer& renderer)
	{
		for (int i = 0; i < GetChildCount(); i++)
			GetChild(i)->Render(renderer);
	}


	void Node::Visit(const std::function<void(Node&)>& function)
	{
		PreVisit(function);
	}


	void Node::PreVisit(const std::function<void(Node&)>& function)
	{
		function(*this);
		for (auto& child : mChildren)
		{
			child->PreVisit(function);
		}
	}


	void Node::PostVisit(const std::function<void(Node&)>& function)
	{
		for (auto& child : mChildren)
		{
			child->PostVisit(function);
		}
		function(*this);
	}


	Core::Optional<std::string> Node::GetId() const
	{
		return mId;
	}


	void Node::SetId(const std::string& id)
	{
		mId = id;
	}


	void Node::RemoveId()
	{
		mId.Reset();
	}


	Core::Optional<Node*> Node::FindById(std::string id)
	{
		// Check ourselves
		if (mId == id) return this;

		// Do depth first search
		for (const auto& child : mChildren)
		{
			if (auto result = child->FindById(id))
			{
				return result;
			}
		}

		// Nothing found
		return Core::NullOpt;
	}


	bool Node::HasAncestor(Node& node)
	{
		bool hasAncestor = false;

		node.Visit([&, this](Node& child)
		{
			if (static_cast<Node*>(this) == &child) hasAncestor = true;
		});

		return hasAncestor;
	}


	Core::ReflexivePointer<Node> Node::GetParent() const
	{
		return mParent;
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
		node->mParent = GetReflexivePointer();
		mChildren.emplace_back(std::move(node));
		return mChildren.back();
	}


	std::shared_ptr<Node> Node::PrependChild(std::shared_ptr<Node> node)
	{
		Core::Assert(node->GetParent() == nullptr);
		node->mParent = GetReflexivePointer();
		mChildren.emplace(mChildren.begin(), std::move(node));
		return mChildren.front();
	}


	std::shared_ptr<Node> Node::InsertChild(size_t index, std::shared_ptr<Node> node)
	{
		Core::Assert(node->GetParent() == nullptr);
		Core::Assert(index <= GetChildCount());
		node->mParent = GetReflexivePointer();
		mChildren.emplace(mChildren.begin() + static_cast<int>(index), std::move(node));
		return mChildren[index];
	}


	Core::Math::Vec2f Node::GetPosition() const
	{
		auto parentPosition = mParent ? mParent->GetPosition() : Core::Math::Vec2f();
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
		auto parentScale = mParent ? mParent->GetScale() : Core::Math::Vec2f(1.0f, 1.0f);
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


	bool Node::IsVisible() const
	{
		return mVisible;
	}


	void Node::SetVisible(bool visible)
	{
		mVisible = visible;
	}


	bool Node::ContainsPoint(Core::Math::Vec2f screenPosition)
	{
		auto relative = screenPosition - GetPosition();
		const Core::Math::Vec2f size = GetSize();
		return relative[0] >= 0.0f && relative[0] <= size[0] && relative[1] >= 0.0f && relative[1] <= size[1];
	}


	uint32_t Node::GetDepth() const
	{
		uint32_t depth = 0;
		auto node = GetReflexivePointer();
		while (node) {
			node = node->GetParent();
			++depth;
		}
		return depth;
	}


	void Node::AddEventListener(std::unique_ptr<EventListener> eventListener)
	{
		mEventListeners.emplace_back(std::move(eventListener));
	}


	std::vector<EventListener*> Node::GatherEventListeners(const Graphics::Window::Event& event)
	{
		std::vector<EventListener*> listeners;
		listeners.reserve(mEventListeners.size());

		for (auto& listener : mEventListeners)
		{
			if (listener->InterestedIn(event))
			{
				listeners.push_back(listener.get());
			}
		}
		return listeners;
	}
}
