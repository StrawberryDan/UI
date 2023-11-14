#pragma once
//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// Strawberry Graphics
#include "Strawberry/Graphics/Event.hpp"
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
		using Predicate = std::function<bool(const Graphics::Window::Event&)>;


		EventListener(Predicate predicate);
		virtual ~EventListener() = default;


		virtual bool Process(const Graphics::Window::Event&) = 0;


		bool InterestedIn(const Graphics::Window::Event& event) const;


	protected:
		Predicate mPredicate;
	};
}
