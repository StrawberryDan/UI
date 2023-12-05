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


	void Animation::DoCallback()
	{
		if (mFinishCallback) mFinishCallback();
	}
}
