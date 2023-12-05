#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include <functional>
#include <memory>


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class Node;


	class Animation
	{
	public:
		virtual ~Animation() = default;


		virtual void Update(float deltaTime, Node& node) = 0;
		virtual bool IsFinished() = 0;


		std::unique_ptr<Animation> GetNextAnimation();


		void DoCallback();


	protected:
		template <std::derived_from<Animation> T>
		void SetNextAnimation(T&& animation)
		{
			mNextAnimation = std::make_unique<T>(std::forward<T>(animation));
		}


	private:
		std::unique_ptr<Animation> mNextAnimation;
		std::function<void()>      mFinishCallback;
	};
}
