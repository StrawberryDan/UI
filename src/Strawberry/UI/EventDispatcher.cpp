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
		if (auto key = event.Value<Graphics::Window::Events::Key>())
		{
			Dispatch(key.Value());
		}

		if (auto text = event.Value<Graphics::Window::Events::Text>())
		{
			Dispatch(text.Value());
		}

		if (auto mouseButton = event.Value<Graphics::Window::Events::MouseButton>())
		{
			Dispatch(mouseButton.Value());
		}

		if (auto mouseMoveEvent = event.Value<Graphics::Window::Events::MouseMove>())
		{
			Dispatch(mouseMoveEvent.Value());
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


	void EventDispatcher::Dispatch(const Graphics::Window::Events::Text& event)
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


	void EventDispatcher::Dispatch(const Graphics::Window::Events::MouseButton& event)
	{
		if (auto target = FindNodeAtPoint(event.position))
		{
			while(target)
			{
				auto listeners = target->GatherEventListeners(event);

				for (auto listener : listeners)
				{
					bool continuePropagation = (*listener).Process(event);
					if (!continuePropagation) return;
				}

				target = target->GetParent();
			}
		}
	}


	void EventDispatcher::Dispatch(const Graphics::Window::Events::MouseMove& event)
	{
		if (auto target = FindNodeAtPoint(event.position))
		{
			while(target)
			{
				auto listeners = target->GatherEventListeners(event);

				for (auto listener : listeners)
				{
					bool continuePropagation = (*listener).Process(event);
					if (!continuePropagation) return;
				}

				target = target->GetParent();
			}
		}
	}


	Core::Optional<Node*> EventDispatcher::FindNodeAtPoint(Core::Math::Vec2f screenPosition)
	{
		Core::Optional<Node*> result;

		mNode->PostVisit([&](Node& node) {
			if (!result && node.ContainsPoint(screenPosition)) result = &node;
		});

		return result;
	}
}