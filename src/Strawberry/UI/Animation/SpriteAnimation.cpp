#include "SpriteAnimation.hpp"

#include "Strawberry/UI/Sprite.hpp"


//======================================================================================================================
//  Method Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::UI
{
	static std::vector<unsigned int> FrameRangeToVector(unsigned int startIndex, unsigned int frameCount)
	{
		std::vector<unsigned int> frames;
		frames.reserve(frameCount);
		for (int i = startIndex; i < startIndex + frameCount; i++)
		{
			frames.emplace_back(i);
		}
		Core::AssertEQ(frames.size(), frameCount);
		return frames;
	}


	SpriteAnimation::SpriteAnimation(const std::vector<unsigned>& frames, float frameRate)
		: mFrameTime(1.0f / frameRate)
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
			mFrameIndex += 1;
			time -= mFrameTime;
		}

		dynamic_cast<Sprite*>(&node)->GetSprite().SetSpriteIndex(mFrames[mFrameIndex]);
	}


	bool SpriteAnimation::IsFinished()
	{
		return mFrameIndex >= mFrames.size();
	}
}
