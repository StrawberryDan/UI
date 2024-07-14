#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Nodes/Node.hpp"
#include "Events/Dispatcher.hpp"
// Standard Library
#include <concepts>
#include <memory>


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class Frame
			: public Core::EnableReflexivePointer
	{
		friend class Events::Dispatcher;

		public:
			Frame();
			Frame(std::unique_ptr<Node>&& root);


			Frame(std::derived_from<Node> auto node)
			{
				SetRoot(std::move(node));
			}


			virtual bool Dispatch(const Window::Event& event);
			virtual void Update(Core::Seconds deltaTime);
			virtual void Render(Renderer& renderer);


			Core::Optional<Node*> GetRoot();
			Node*                 SetRoot(std::unique_ptr<Node> node);


			auto SetRoot(std::derived_from<Node> auto node)
			{
				using NodeType = std::decay_t<decltype(node)>;
				return static_cast<decltype(node)*>(SetRoot(std::make_unique<NodeType>(std::move(node))));
			}


			bool IsVisible() const;
			void SetVisible(bool visible);

		protected:
			Core::ReflexivePointer<Node> GetFocus();
			void                         SetFocus(Node& node);

		private:
			std::unique_ptr<Node>        mRoot;
			Events::Dispatcher           mEventDispatcher;
			Core::ReflexivePointer<Node> mFocus;

			bool mVisible = true;
	};
}
