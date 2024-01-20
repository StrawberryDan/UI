#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "CallbackEventListener.hpp"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
    class MouseButtonEventListener
        : public CallbackEventListener
    {
    public:
        using Callback = std::function<void(const Window::Events::MouseButton&)>;


        MouseButtonEventListener(Window::Input::MouseButton button, Window::Input::KeyAction action, Callback callback);
        MouseButtonEventListener(Callback callback);
    };
}
