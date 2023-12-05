#include "SpriteAnimation.hpp"
#include "Strawberry/UI/Sprite.hpp"
#include <numeric>


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	static std::vector<unsigned int> FrameRangeToVector(unsigned int startIndex, unsigned int frameCount)
	{

		std::vector<unsigned int> frames(frameCount, 0);
		std::iota(frames.begin(), frames.end(), startIndex);
		Core::AssertEQ(frames.size(), frameCount);
		return frames;
	}


	SpriteAnimation::SpriteAnimation(const std::vector<unsigned>& frames, float frameRate)
		: mFrameIndex(0)
		, mFrameTime(1.0f / frameRate)
		, mFrames(frames)
	{
		mFrames.shrink_to_fit();
	}


	SpriteAnimation::SpriteAnimation(unsigned startIndex, unsigned frameCount, float frameRate)
		: SpriteAnimation(FrameRangeToVector(startIndex, frameCount), frameRate)
	{}


	void SpriteAnimation::Update(float deltaTime, Node& node)
	{
		Core::AssertNEQ(dynamic_cast<Sprite*>(&node), nullptr);

		auto time = mFrameClock.Restart();
		while (time > mFrameTime)
		{
			mFrameIndex = std::min<unsigned int>(mFrames.size() - 1, mFrameIndex + 1);
			time -= mFrameTime;
		}

		Core::Assert(mFrameIndex < mFrames.size());
		dynamic_cast<Sprite*>(&node)->GetSprite().SetSpriteIndex(mFrames[mFrameIndex]);
	}


	bool SpriteAnimation::IsFinished()
	{
		return mFrameIndex >= mFrames.size() - 1;
	}
}
