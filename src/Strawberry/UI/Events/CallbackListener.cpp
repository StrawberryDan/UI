//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "CallbackListener.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI::Events
{
	CallbackListener::CallbackListener(Listener::Predicate predicate, CallbackListener::Callback callback)
		: Listener(std::move(predicate))
		, mCallback(std::move(callback))
	{}


	bool CallbackListener::Process(const Window::Event& event)
	{
		Core::Assert(mPredicate(event));
		return mCallback(event);
	}
}