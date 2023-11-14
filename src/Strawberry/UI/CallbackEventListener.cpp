//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "CallbackEventListener.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	CallbackEventListener::CallbackEventListener(EventListener::Predicate predicate, CallbackEventListener::Callback callback)
		: EventListener(std::move(predicate))
		, mCallback(std::move(callback))
	{}


	bool CallbackEventListener::Process(const Graphics::Window::Event& event)
	{
		Core::Assert(mPredicate(event));
		return mCallback(event);
	}
}