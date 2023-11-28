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
        using Callback = std::function<void(const Graphics::Window::Events::MouseButton&)>;


        MouseButtonEventListener(Graphics::Input::MouseButton button, Graphics::Input::KeyAction action, Callback callback);
        MouseButtonEventListener(Callback callback);
    };
}
