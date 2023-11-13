//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "EventDispatcher.hpp"
#include "Frame.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	EventDispatcher::EventDispatcher(Frame& frame, Node& node)
		: mFrame(frame)
		, mNode(node)
	{}


	void EventDispatcher::Dispatch(const Graphics::Window::Event& event)
	{
		if (auto keyEvent = event.Value<Graphics::Window::Events::Key>())
		{
			Dispatch(keyEvent.Value());
		}
	}


	void EventDispatcher::Dispatch(const Graphics::Window::Events::Key& event)
	{
		if (auto focus = mFrame->GetFocus())
		{
			while(focus)
			{
				auto listeners = focus->GatherEventListeners(event);

				for (auto listener : listeners)
				{
					bool continuePropagation = (*listener).Process(event);
					if (!continuePropagation) return;
				}

				focus = focus->GetParent();
			}
		}
	}
}