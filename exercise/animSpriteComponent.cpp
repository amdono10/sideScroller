#include "animSpriteComponent.hpp"
#include "math.hpp"

AnimSpriteComponent::AnimSpriteComponent(Entity* owner, int drawOrder):
	SpriteComponent(owner, drawOrder),
	mCurrFrame(0.0f),
	mAnimFPS(24.0f),
	mFinished(false)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
	
	// done updating if mFinished (set by non-looping animations)
	if (mAnimTextures.size() == 0 || mFinished)
	{
		return;
	}
	
	// Default range: the whole texture vector, looping.
	// This keeps old callers like Ship, who only ever call
	// SetAnimTextures, working exactly as before.)
	int startFrame = 0;
	int endFrame = static_cast<int>(mAnimTextures.size()) - 1;
	bool looping = true;	

	// set values based on Animation name
	auto iter = mAnimations.find(mCurrAnimName);
	if (iter != mAnimations.end())
	{
		startFrame = iter->second.mStartFrame;
		endFrame = iter->second.mEndFrame;
		looping = iter->second.mLooping;
	}	

	int numFrames = endFrame - startFrame + 1;
	
	// update the current frame based on frame rate and delta time
	mCurrFrame += mAnimFPS * deltaTime;

	// how far past the start of this animation's range are we?
	float relFrame = mCurrFrame - startFrame;
	
	if (relFrame >= numFrames)
	{	
		if (looping)
		{
			// wrap back into this animation's range
			while (relFrame >= numFrames)
			{
				relFrame -= numFrames;
			}
			mCurrFrame = startFrame + relFrame;
		}
		else
		{
			// non-looping: clamp to the last frame and stop advancing
			mCurrFrame = static_cast<float>(endFrame);
			mFinished = true;
		}
	}

	// set the current texture
	SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// set the active texture to first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}

void AnimSpriteComponent::AddAnimation(const std::string& name, int startFrame, int endFrame, bool looping)
{
	Animation anim;
	anim.mStartFrame = startFrame;
	anim.mEndFrame = endFrame;
	anim.mLooping = looping;
	mAnimations.emplace(name, anim);
}

void AnimSpriteComponent::SetAnimation(const std::string& name)
{
	auto iter = mAnimations.find(name);
	if (iter == mAnimations.end())
	{
		return;	// unable to find animation name in mAnimations
	}
	
	mCurrAnimName = name;
	mFinished = false;
	mCurrFrame = static_cast<float>(iter->second.mStartFrame);
	SetTexture(mAnimTextures[iter->second.mStartFrame]);
}
