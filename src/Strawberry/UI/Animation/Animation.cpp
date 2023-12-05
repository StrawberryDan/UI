#include "Animation.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	std::unique_ptr<Animation> Animation::GetNextAnimation()
	{
		return std::move(mNextAnimation);
	}


	void Animation::SetCallback(std::function<void()> function)
	{
		mFinishCallback = std::move(function);
	}


	void Animation::DoCallback()
	{
		if (mFinishCallback) mFinishCallback();
	}
}
