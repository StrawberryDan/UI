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


		bool Dispatch(const Graphics::Window::Event& event);


	protected:
		bool Dispatch(const Graphics::Window::Events::Key& event);
		bool Dispatch(const Graphics::Window::Events::Text& event);
		bool Dispatch(const Graphics::Window::Events::MouseButton& event);
		bool Dispatch(const Graphics::Window::Events::MouseMove& event);


		Core::Optional<Node*> FindNodeAtPoint(Core::Math::Vec2f screenPosition);


	private:
		Core::ReflexivePointer<Frame> mFrame;
		Core::ReflexivePointer<Node> mNode;
	};
}