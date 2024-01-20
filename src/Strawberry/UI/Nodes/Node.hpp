#pragma once


#include "..\Events\Listener.hpp"
#include "Strawberry/UI/Animation/Animation.hpp"
// Strawberry Graphics
#include "Strawberry/Window/Event.hpp"
// Strawberry Core
#include "Strawberry/Core/Math/Vector.hpp"
#include "Strawberry/Core/Types/ReflexivePointer.hpp"
#include "Strawberry/Core/Types/Optional.hpp"
#include "Strawberry/Core/Timing/Clock.hpp"
// Standard Library
#include <memory>
#include <vector>


namespace Strawberry::UI
{
	class Renderer;
	namespace Events { class Listener; }


	class Node
		: public Core::EnableReflexivePointer<Node>
	{
	public:
		Node();

		Node(const Node&) = delete;
		Node& operator=(const Node&) = delete;
		Node(Node&&) = default;
		Node& operator=(Node&&) = default;

		virtual ~Node() = default;


		virtual void Update(Core::Seconds deltaTime);


		virtual void Render(Renderer& renderer);


		void Visit(const std::function<void(Node&)>& function);
		void PreVisit(const std::function<void(Node&)>& function);
		void PostVisit(const std::function<void(Node&)>& function);


		[[nodiscard]] Core::Optional<std::string> GetId() const;
		void SetId(const std::string& id);
		void RemoveId();


		Core::Optional<Node*> FindById(std::string id);


		[[nodiscard]] bool HasAncestor(Node& node);


		[[nodiscard]] Core::ReflexivePointer<Node> GetParent() const;

		template <std::derived_from<Node> T>
		[[nodiscard]] Core::Optional<T*> GetParent() const
		{
			return dynamic_cast<T*>(GetParent().Get());
		}

		[[nodiscard]] Core::ReflexivePointer<Node> CommonAncestor(const Node& other) const;

		template <std::derived_from<Node> T>
		[[nodiscard]] Core::Optional<T*> CommonAncestor(const Node& other) const
		{
			return dynamic_cast<T*>(CommonAncestor(other).Get());
		}


		[[nodiscard]] size_t GetChildCount() const;

		[[nodiscard]] std::shared_ptr<Node> GetChild(size_t index);


		template<std::derived_from<Node> T>
		std::shared_ptr<T> AppendChild(T&& node)
		{
			auto ptr = AppendChild(std::make_shared<T>(std::forward<T&&>(node)));
			return std::static_pointer_cast<T>(ptr);
		}


		template <std::derived_from<Node> T, typename... Args>
		std::shared_ptr<T> AppendChild(Args&&... args)
		{
			auto node = std::make_shared<T>(std::forward<Args&&>(args)...);
			AppendChild(std::static_pointer_cast<Node>(node));
			return node;
		}

		std::shared_ptr<Node> AppendChild(std::shared_ptr<Node> node);
		std::shared_ptr<Node> PrependChild(std::shared_ptr<Node> node);
		std::shared_ptr<Node> InsertChild(size_t index, std::shared_ptr<Node> node);


		void ClearChildren();


		void AddEventListener(std::unique_ptr<Events::Listener> eventListener);
		void AddEventListener(std::derived_from<Events::Listener> auto eventListener)
		{
			AddEventListener(std::make_unique<std::decay_t<decltype(eventListener)>>(std::forward<decltype(eventListener)>(eventListener)));
		}

		std::vector<Events::Listener*> GatherEventListeners(const Events::Event& event);


		[[nodiscard]] Core::Math::Vec2f GetPosition() const;
		[[nodiscard]] Core::Math::Vec2f GetLocalPosition() const;

		[[nodiscard]] Core::Math::Vec2f GetScale() const;
		[[nodiscard]] Core::Math::Vec2f GetLocalScale() const;


		void SetLocalPosition(Core::Math::Vec2f position);
		void Move(Core::Math::Vec2f offset);
		void SetLocalScale(Core::Math::Vec2f scale);


		bool IsVisible() const;
		void SetVisible(bool visible);


		uint32_t GetDepth() const;


		void AddAnimation(std::unique_ptr<Animation> animation);
		bool IsAnimating() const;


	private:
		Core::Optional<std::string> mId;


		Core::ReflexivePointer<Node> mParent;
		std::vector<std::shared_ptr<Node>> mChildren;

		std::vector<std::unique_ptr<Events::Listener>> mEventListeners;

		Core::Math::Vec2f mLocalPosition = Core::Math::Vec2f(0.0f, 0.0f);
		Core::Math::Vec2f mLocalScale    = Core::Math::Vec2f(1.0f, 1.0f);

		bool mVisible = true;

		std::vector<std::unique_ptr<Animation>> mActiveAnimations;
	};
}
