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
	{
	public:
		Frame();
		Frame(std::unique_ptr<Node>&& root);
		Frame(std::derived_from<Node> auto&& node)
		{
			AddRoot(std::forward<decltype(node)>(node));
		}


		void Update(float deltaTime);
		void Render(Renderer& renderer);


		uint32_t GetRootCount() const;
		Node& GetRoot(uint32_t index);
		void AddRoot(std::unique_ptr<Node>&& node);
		void AddRoot(std::derived_from<Node> auto&& node)
		{
			AddRoot(std::make_unique<std::decay_t<decltype(node)>>(std::forward<decltype(node)>(node)));
		}


	private:
		std::vector<std::unique_ptr<Node>> mRoots;
		std::vector<EventDispatcher> mEventDispatchers;
	};
}

