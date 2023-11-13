//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "EventListener.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	EventListener::EventListener(EventListener::Predicate predicate)
		: mPredicate(std::move(predicate))
	{}


	bool EventListener::InterestedIn(const Graphics::Window::Event& event) const
	{
		return mPredicate(event);
	}
}