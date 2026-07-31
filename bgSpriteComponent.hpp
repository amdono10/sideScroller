#ifndef _BG_SPRITE_COMPONENT_HPP
#define _BG_SPRITE_COMPONENT_HPP

#include "spriteComponent.hpp"
#include <vector>
#include "math.hpp"

class BGSpriteComponent : public SpriteComponent
{
public:
	// Set draw order to default to lower (so that it's in the background)
	BGSpriteComponent(class Entity* owner, int drawOrder = 10);
	// update/draw overridden from parent
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;
	// set the textures used for the background
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);
	// get/set screen size and scroll speed
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }

private:
	// struct to encapsulate each bg image and its offset
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffset;
	};
	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	float mScrollSpeed;
};

#endif	// _BG_SPRITE_COMPONENT_HPP