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
namespace Strawberry::UI
{
	class EventListener
	{
	public:
		using Predicate = std::function<bool(const Window::Event&)>;


		EventListener(Predicate predicate);
		virtual ~EventListener() = default;


		virtual bool Process(const Window::Event&) = 0;


		bool InterestedIn(const Window::Event& event) const;


	protected:
		Predicate mPredicate;
	};
}
