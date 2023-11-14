#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Node.hpp"
#include "EventDispatcher.hpp"
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


		void Update(Core::Seconds deltaTime);
		void Render(Renderer& renderer);


		uint32_t GetRootCount() const;
		Node& GetRoot(uint32_t index);
		void AddRoot(std::unique_ptr<Node> node);
		void AddRoot(std::derived_from<Node> auto node)
		{
			using NodeType = std::decay_t<decltype(node)>;
			AddRoot(std::make_unique<NodeType>(std::move(node)));
		}


		bool Dispatch(const Graphics::Window::Event& event);


	protected:
		Core::ReflexivePointer<Node> GetFocus();
		void SetFocus(Node& node);


	private:
		std::vector<std::unique_ptr<Node>> mRoots;
		std::vector<EventDispatcher> mEventDispatchers;
		Core::ReflexivePointer<Node> mFocus;
	};
}

