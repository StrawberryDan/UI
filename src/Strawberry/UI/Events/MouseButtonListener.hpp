#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "CallbackListener.hpp"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI::Events
{
	class MouseButtonListener
			: public CallbackListener
	{
	public:
		using Callback = std::function<void(const Window::Events::MouseButton&)>;


		MouseButtonListener(Callback callback);
		MouseButtonListener(Window::Input::MouseButton button, Window::Input::KeyAction action, Callback callback);
	};
}
