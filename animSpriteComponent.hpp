#ifndef _ANIM_SPRITE_COMPONENT_HPP
#define _ANIM_SPRITE_COMPONENT_HPP

#include "spriteComponent.hpp"
#include <vector>

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Entity* owner, int drawOrder = 100);
	// update animation every fram (overridden from component)
	void Update(float deltaTime) override;
	// Set the textures use for animation
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	// set/get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

private:
	// All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;
	// Current frame displayed
	float mCurrFrame;
	// Animation frame rate
	float mAnimFPS;
};

#endif	// _ANIM_SPRITE_COMPONENT_HPP