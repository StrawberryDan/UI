#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Listener.hpp"
#include "Event.hpp"
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
		using Callback = std::function<bool(const Event&)>;


		CallbackListener(Listener::Predicate predicate, Callback callback);


		virtual bool Process(const Event& event) override;


	private:
		Callback mCallback;
	};
}