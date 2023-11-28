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
	MouseButtonEventListener::MouseButtonEventListener(Graphics::Input::MouseButton button,
	                                                   Graphics::Input::KeyAction action,
	                                                   Callback callback)
		: CallbackEventListener([=](const Graphics::Window::Event& event)
		{
			return event.IsType<Graphics::Window::Events::MouseButton>()
			       && event.Value<Graphics::Window::Events::MouseButton>()->button == button
			       && event.Value<Graphics::Window::Events::MouseButton>()->action == action;
		}, [=](const Graphics::Window::Event& e){ callback(*e.Value<Graphics::Window::Events::MouseButton>()); return false; })
	{}


	MouseButtonEventListener::MouseButtonEventListener(Callback callback)
		: MouseButtonEventListener(Graphics::Input::MouseButton::Left, Graphics::Input::KeyAction::Press, std::move(callback))
	{}
}
