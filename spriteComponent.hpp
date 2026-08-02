#ifndef _SPRITE_COMPONENT_HPP
#define _SPRITE_COMPONENT_HPP

#include "component.hpp"
#include <SDL3/SDL.h>

class SpriteComponent : public Component
{
public:
	// lower draw order corresponds with further back
	SpriteComponent(class Entity* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

protected:
	SDL_Texture* mTexture;
	int mDrawOrder;
	float mTexWidth;
	float mTexHeight;
};

#endif // _SPRITE_COMPONENT_HPP