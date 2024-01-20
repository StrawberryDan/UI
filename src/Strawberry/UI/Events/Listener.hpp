#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// Strawberry Graphics
#include "Strawberry/UI/Event.hpp"
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
		using Predicate = std::function<bool(const Event&)>;


		Listener(Predicate predicate);
		virtual ~Listener() = default;


		virtual bool Process(const Event&) = 0;


		bool InterestedIn(const Event& event) const;


	protected:
		Predicate mPredicate;
	};
}
