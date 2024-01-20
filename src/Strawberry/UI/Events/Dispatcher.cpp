//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Dispatcher.hpp"
#include "Strawberry/UI/Frame.hpp"
#include "Strawberry/UI/Nodes/RectangularNode.hpp"
#include "Listener.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI::Events
{
	Dispatcher::Dispatcher(Frame& frame)
		: mFrame(frame)
	{}


	bool Dispatcher::Dispatch(const Window::Event& event)
	{
		bool shouldPropagate = true;

		if (auto key = event.Value<Window::Events::Key>())
		{
			shouldPropagate = shouldPropagate && Dispatch(key.Value());
		}

		if (auto text = event.Value<Window::Events::Text>())
		{
			shouldPropagate = shouldPropagate && Dispatch(text.Value());
		}

		if (auto mouseButton = event.Value<Window::Events::MouseButton>())
		{
			shouldPropagate = shouldPropagate && Dispatch(mouseButton.Value());
		}

		if (auto mouseMoveEvent = event.Value<Window::Events::MouseMove>())
		{
			shouldPropagate = shouldPropagate && Dispatch(mouseMoveEvent.Value());
		}


		// Attempt to create and dispatch advanced events
		if (shouldPropagate)
		{
			for (auto advancedEvent : mTransformer.Transform(event))
			{
				shouldPropagate = shouldPropagate && Dispatch(advancedEvent);
				if (!shouldPropagate) break;
			}
		}


		return shouldPropagate;
	}


	bool Dispatcher::Dispatch(const Window::Events::Key& event)
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


	bool Dispatcher::Dispatch(const Window::Events::Text& event)
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


	bool Dispatcher::Dispatch(const Window::Events::MouseButton& event)
	{
		if (auto target = FindNodeAtPoint(event.position))
		{
			// Send an unfocus event if required.
			// Send an unfocus event if there is a current focus and that focus is not the target
			if (auto focus = mFrame->GetFocus(); focus && focus != target->GetReflexivePointer()) Dispatch(Unfocus());
			// Update focus and send event to notify receiving node
			mFrame->SetFocus(*target.Value());
			Dispatch(Focus());


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


	bool Dispatcher::Dispatch(const Window::Events::MouseMove& event)
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


	bool Dispatcher::Dispatch(const Event& event)
	{
		return true;
	}


	bool Dispatcher::Dispatch(const Events::Focus& event)
	{
		if (mFrame->GetFocus())
		{
			for (auto listener : mFrame->GetFocus()->GatherEventListeners(event))
			{
				bool continuePropagation = listener->Process(event);
				if (!continuePropagation) return false;
			}
		}

		return true;
	}


	bool Dispatcher::Dispatch(const Events::Unfocus& event)
	{
		if (mFrame->GetFocus())
		{
			for (auto listener : mFrame->GetFocus()->GatherEventListeners(event))
			{
				bool continuePropagation = listener->Process(event);
				if (!continuePropagation) return false;
			}
		}

		return true;
	}


	Core::Optional<Node*> Dispatcher::FindNodeAtPoint(Core::Math::Vec2f screenPosition)
	{
		Core::Optional<Node*> result;

		if (auto root = mFrame->GetRoot())
			root->PostVisit([&](Node& node) {
				auto asSized = dynamic_cast<UI::RectangularNode*>(&node);
				if (!asSized) return;

				const bool isAncestor = result && result->HasAncestor(node);
				if (!isAncestor && asSized->IsVisible() && asSized->ContainsPoint(screenPosition))
				{
					result = &node;
				}
			});

		return result;
	}
}