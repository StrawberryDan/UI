//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "MouseButtonEventListener.hpp"
// Standard Library
#include <utility>


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	MouseButtonEventListener::MouseButtonEventListener(Window::Input::MouseButton button,
	                                                   Window::Input::KeyAction action,
	                                                   Callback callback)
		: CallbackEventListener([=](const Window::Event& event)
		{
			return event.IsType<Window::Events::MouseButton>()
			       && event.Value<Window::Events::MouseButton>()->button == button
			       && event.Value<Window::Events::MouseButton>()->action == action;
		}, [=](const Window::Event& e){ callback(*e.Value<Window::Events::MouseButton>()); return false; })
	{}


	MouseButtonEventListener::MouseButtonEventListener(Callback callback)
		: MouseButtonEventListener(Window::Input::MouseButton::Left, Window::Input::KeyAction::Press, std::move(callback))
	{}
}
