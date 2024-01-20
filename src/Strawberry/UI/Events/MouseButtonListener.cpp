//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "MouseButtonListener.hpp"
// Standard Library
#include <utility>


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI::Events
{
	MouseButtonListener::MouseButtonListener(Callback callback)
		: MouseButtonListener(Window::Input::MouseButton::Left, Window::Input::KeyAction::Press, std::move(callback))
	{}


	MouseButtonListener::MouseButtonListener(Window::Input::MouseButton button,
											 Window::Input::KeyAction action,
											 Callback callback)
		: CallbackListener([=](const Event& event)
		{
			return event.IsType<Window::Events::MouseButton>()
			       && event.Value<Window::Events::MouseButton>()->button == button
			       && event.Value<Window::Events::MouseButton>()->action == action;
		}, [=](const Event& e){ callback(*e.Value<Window::Events::MouseButton>()); return false; })
	{}
}
