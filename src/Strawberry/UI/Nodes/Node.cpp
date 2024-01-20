#include "Node.hpp"
#include "..\Events\Listener.hpp"
#include <Strawberry/Core/Assert.hpp>


namespace Strawberry::UI
{
	Node::Node()
		: mParent()
	{}


	void Node::Update(Core::Seconds deltaTime)
	{
		for (auto& animation : mActiveAnimations)
		{
			animation->Update(deltaTime, *this);

			if (animation->IsFinished())
			{
				animation->DoCallback();

				if (auto nextAnimation = animation->GetNextAnimation())
				{
					animation = std::move(nextAnimation);
				}
			}
		}


		erase_if(mActiveAnimations, [](auto& x) { return x->IsFinished(); });


		for (auto& child : mChildren)
		{
			child->Update(deltaTime);
		}
	}


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


	Core::ReflexivePointer<Node> Node::CommonAncestor(const Node& other) const
	{
		auto generateParentChain = [](const Node& node)
		{
			std::vector<Core::ReflexivePointer<Node>> parentChain { node.GetReflexivePointer() };
			while (auto parent = parentChain.back()->GetParent())
			{
				parentChain.emplace_back(parent);
			}
			return parentChain;
		};


		auto myParentChain = generateParentChain(*this);
		auto theirParentChain = generateParentChain(other);


		for (auto node : myParentChain)
		{
			if (auto corresponding = std::ranges::find(theirParentChain.begin(), theirParentChain.end(), node); corresponding != theirParentChain.end())
			{
				return *corresponding;
			}
		}

		return {};
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


	void Node::ClearChildren()
	{
		mChildren.clear();
	}


	Core::Math::Vec2f Node::GetPosition() const
	{
		auto parentPosition = mParent ? mParent->GetPosition() : Core::Math::Vec2f();
		auto parentScale = mParent ? mParent->GetLocalScale() : Core::Math::Vec2f(1.0f, 1.0f);
		return parentPosition + parentScale * GetLocalPosition();
	}


	Core::Math::Vec2f Node::GetLocalPosition() const
	{
		return mLocalPosition;
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
		mLocalPosition = position.Map([](auto x) { return std::roundf(x); });
	}


	void Node::Move(Core::Math::Vec2f offset)
	{
		SetLocalPosition(GetLocalPosition() + offset);
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


	void Node::AddAnimation(std::unique_ptr<Animation> animation)
	{
		mActiveAnimations.emplace_back(std::move(animation));
	}


	bool Node::IsAnimating() const
	{
		return !mActiveAnimations.empty();
	}


	void Node::AddEventListener(std::unique_ptr<Events::Listener> eventListener)
	{
		mEventListeners.emplace_back(std::move(eventListener));
	}


	std::vector<Events::Listener*> Node::GatherEventListeners(const Events::Event& event)
	{
		std::vector<Events::Listener*> listeners;
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
