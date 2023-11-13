#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Node.hpp"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class Frame;


	class EventDispatcher
	{
	public:
		EventDispatcher(Frame& frame, Node& node);


		void Dispatch(const Graphics::Window::Event& event);


	protected:
		void Dispatch(const Graphics::Window::Events::Key& event);


	private:
		Core::ReflexivePointer<Frame> mFrame;
		Core::ReflexivePointer<Node> mNode;
	};
}