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


	bool EventDispatcher::Dispatch(const Graphics::Window::Event& event)
	{
		if (auto key = event.Value<Graphics::Window::Events::Key>())
		{
			return Dispatch(key.Value());
		}

		if (auto text = event.Value<Graphics::Window::Events::Text>())
		{
			return Dispatch(text.Value());
		}

		if (auto mouseButton = event.Value<Graphics::Window::Events::MouseButton>())
		{
			return Dispatch(mouseButton.Value());
		}

		if (auto mouseMoveEvent = event.Value<Graphics::Window::Events::MouseMove>())
		{
			return Dispatch(mouseMoveEvent.Value());
		}

		return true;
	}


	bool EventDispatcher::Dispatch(const Graphics::Window::Events::Key& event)
	{
		if (auto focus = mFrame->GetFocus())
		{
			while(focus)
			{
				auto listeners = focus->GatherEventListeners(event);

				for (auto listener : listeners)
				{
					bool continuePropagation = (*listener).Process(event);
					if (!continuePropagation) return false;
				}

				focus = focus->GetParent();
			}
		}

		return true;
	}


	bool EventDispatcher::Dispatch(const Graphics::Window::Events::Text& event)
	{
		if (auto focus = mFrame->GetFocus())
		{
			while(focus)
			{
				auto listeners = focus->GatherEventListeners(event);

				for (auto listener : listeners)
				{
					bool continuePropagation = (*listener).Process(event);
					if (!continuePropagation) return false;
				}

				focus = focus->GetParent();
			}
		}

		return true;
	}


	bool EventDispatcher::Dispatch(const Graphics::Window::Events::MouseButton& event)
	{
		if (auto target = FindNodeAtPoint(event.position))
		{
			mFrame->SetFocus(*target.Value());
			while(target)
			{
				auto listeners = target->GatherEventListeners(event);

				for (auto listener : listeners)
				{
					bool continuePropagation = (*listener).Process(event);
					if (!continuePropagation) return false;
				}

				target = target->GetParent();
			}
		}

		return true;
	}


	bool EventDispatcher::Dispatch(const Graphics::Window::Events::MouseMove& event)
	{
		if (auto target = FindNodeAtPoint(event.position))
		{
			while(target)
			{
				auto listeners = target->GatherEventListeners(event);

				for (auto listener : listeners)
				{
					bool continuePropagation = (*listener).Process(event);
					if (!continuePropagation) return false;
				}

				target = target->GetParent();
			}
		}

		return true;
	}


	Core::Optional<Node*> EventDispatcher::FindNodeAtPoint(Core::Math::Vec2f screenPosition)
	{
		Core::Optional<Node*> result;

		mNode->PostVisit([&](Node& node) {
			if (!result && node.GetVisible() && node.ContainsPoint(screenPosition))
			{
				result = &node;
			}
		});

		return result;
	}
}