#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Nodes/Node.hpp"
#include "Events/EventDispatcher.hpp"
// Standard Library
#include <concepts>
#include <memory>


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class Frame
		: public Core::EnableReflexivePointer<Frame>
	{
		friend class EventDispatcher;


	public:
		Frame();
		Frame(std::unique_ptr<Node>&& root);
		Frame(std::derived_from<Node> auto node)
		{
			AddRoot(std::move(node));
		}


		virtual bool Dispatch(const Window::Event& event);
		virtual void Update(Core::Seconds deltaTime);
		virtual void Render(Renderer& renderer);


		uint32_t GetRootCount() const;
		Node& GetRoot(uint32_t index);
		Node* AddRoot(std::unique_ptr<Node> node);
		auto AddRoot(std::derived_from<Node> auto node)
		{
			using NodeType = std::decay_t<decltype(node)>;
			return static_cast<decltype(node)*>(AddRoot(std::make_unique<NodeType>(std::move(node))));
		}


		bool IsVisible() const;
		void SetVisible(bool visible);


	protected:
		Core::ReflexivePointer<Node> GetFocus();
		void SetFocus(Node& node);


	private:
		std::vector<std::unique_ptr<Node>> mRoots;
		std::vector<EventDispatcher> mEventDispatchers;
		Core::ReflexivePointer<Node> mFocus;

		bool mVisible = true;
	};
}

