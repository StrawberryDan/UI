#pragma once


//======================================================================================================================
//  Include
//----------------------------------------------------------------------------------------------------------------------
#include "Animation.hpp"
#include "Strawberry/Core/Timing/Clock.hpp"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	class SpriteAnimation
		: public Animation
	{
	public:
		SpriteAnimation(const std::vector<unsigned int>& frames, float frameRate);
		SpriteAnimation(unsigned int startIndex, unsigned int frameCount, float frameRate);


		void Update(float deltaTime, Node& node) override;
		bool IsFinished() override;


	private:
		unsigned int              mFrameIndex;
		float                     mFrameTime;
		std::vector<unsigned int> mFrames;
		float                     mTimeElapsed = 0.0f;
		bool                      mIsFinished = false;
	};
}
