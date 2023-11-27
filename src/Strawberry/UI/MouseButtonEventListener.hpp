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
        MouseButtonEventListener(Graphics::Input::MouseButton button, Graphics::Input::KeyAction action, UI::CallbackEventListener::Callback callback);
        MouseButtonEventListener(UI::CallbackEventListener::Callback callback);
    };
}
