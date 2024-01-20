//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Dispatcher.hpp"
#include "Strawberry/UI/Frame.hpp"
#include "Strawberry/UI/Nodes/RectangularNode.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	Dispatcher::Dispatcher(Frame& frame, Node& node)
		: mFrame(frame)
		, mNode(node)
	{}


	bool Dispatcher::Dispatch(const Window::Event& event)
	{
		if (auto key = event.Value<Window::Events::Key>())
		{
			return Dispatch(key.Value());
		}

		if (auto text = event.Value<Window::Events::Text>())
		{
			return Dispatch(text.Value());
		}

		if (auto mouseButton = event.Value<Window::Events::MouseButton>())
		{
			return Dispatch(mouseButton.Value());
		}

		if (auto mouseMoveEvent = event.Value<Window::Events::MouseMove>())
		{
			return Dispatch(mouseMoveEvent.Value());
		}

		return true;
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


	Core::Optional<Node*> Dispatcher::FindNodeAtPoint(Core::Math::Vec2f screenPosition)
	{
		Core::Optional<Node*> result;

		mNode->PostVisit([&](Node& node) {
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