#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Transformer.hpp"
#include "Strawberry/UI/Nodes/Node.hpp"


//======================================================================================================================
//  Foreward Declarations
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class Frame;
}


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI::Events
{
	class Dispatcher
	{
	public:
		Dispatcher(Frame& frame);


		/// Dispatches an event and returns whether the event should continue to propagate
		bool Dispatch(const Window::Event& event);


	protected:
		bool Dispatch(const Window::Events::Key& event);
		bool Dispatch(const Window::Events::Text& event);
		bool Dispatch(const Window::Events::MouseButton& event);
		bool Dispatch(const Window::Events::MouseMove& event);

		bool Dispatch(const Event& event);


		Core::Optional<Node*> FindNodeAtPoint(Core::Math::Vec2f screenPosition);


	private:
		Core::ReflexivePointer<Frame> mFrame;
		Transformer mTransformer;
	};
}