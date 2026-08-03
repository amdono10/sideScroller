#ifndef _ANIM_SPRITE_COMPONENT_HPP
#define _ANIM_SPRITE_COMPONENT_HPP

#include "spriteComponent.hpp"
#include <vector>
#include <unordered_map>
#include <string>

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Entity* owner, int drawOrder = 100);
	// update animation every fram (overridden from component)
	void Update(float deltaTime) override;
	// Set the textures use for animation
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	
	// Define a named animation as an inclusive [startFrame, endFrame] range
	// into the vector passed to SetAnimTextures, with a looping flag
	void AddAnimation(const std::string& name, int startFrame, int endFrame, bool looping = true);
	// start playing a previously-added named animation, from its name
	void SetAnimation(const std::string& name);
	// True once a non-looping animation has reached its final frame
	bool IsAnimationFinished() const { return mFinished; }	
	
	// set/get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

private:
	// A named animation is just a range of frames into mAnimTextures,
	// and whether it should loop or stop on its last frame
	struct Animation
	{
		int mStartFrame = 0;
		int mEndFrame = 0;
		bool mLooping = true;
	};	

	// All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;
	// Named animations, keyed by name (e.g. "Walking", "Jumping", "Punch")
	std::unordered_map<std::string, Animation> mAnimations;
	// Name of the currently active animation ("" = whole vector, looping)
	std::string mCurrAnimName;
	// Current frame displayed
	float mCurrFrame;
	// Animation frame rate
	float mAnimFPS;	
	// Set to true when a non-looping animation reaches its end frame
	bool mFinished;
};

#endif	// _ANIM_SPRITE_COMPONENT_HPP