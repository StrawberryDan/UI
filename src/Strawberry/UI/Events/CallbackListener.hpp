#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Listener.hpp"
// Standard Library
#include <functional>

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI::Events
{
	class CallbackListener
		: public Listener
	{
	public:
		using Callback = std::function<bool(const Window::Event&)>;


		CallbackListener(Listener::Predicate predicate, Callback callback);


		virtual bool Process(const Window::Event& event) override;


	private:
		Callback mCallback;
	};
}