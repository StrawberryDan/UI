#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "EventListener.hpp"
// Standard Library
#include <functional>

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class CallbackEventListener
		: public EventListener
	{
	public:
		using Callback = std::function<bool(const Graphics::Window::Event&)>;


		CallbackEventListener(EventListener::Predicate predicate, Callback callback);


		virtual bool Process(const Graphics::Window::Event& event) override;


	private:
		Callback mCallback;
	};
}