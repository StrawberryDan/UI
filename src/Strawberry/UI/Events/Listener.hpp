#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// Strawberry Graphics
#include "Strawberry/Window/Event.hpp"
// Standard Library
#include <functional>


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI::Events
{
	class Listener
	{
	public:
		using Predicate = std::function<bool(const Window::Event&)>;


		Listener(Predicate predicate);
		virtual ~Listener() = default;


		virtual bool Process(const Window::Event&) = 0;


		bool InterestedIn(const Window::Event& event) const;


	protected:
		Predicate mPredicate;
	};
}
