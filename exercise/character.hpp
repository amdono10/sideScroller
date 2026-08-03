#ifndef _CHARACTER_HPP
#define _CHARACTER_HPP

#include "entity.hpp"

class Character : public Entity
{
public:
	Character(class Game* game);
	void UpdateEntity(float deltaTime) override;
	void ProcessKeyboard(const bool* state);
	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }

private:
	float mRightSpeed;
	float mDownSpeed;
	
	// which one-shot "action" animation is currently playing, if any.
	// While an action is active, new jump/punch input is ignored so it
	// can't interrupt or restart the animation mid-playback.
	enum class Action
	{
		None,
		Jumping,
		Punching
	};

	class AnimSpriteComponent* mAnimComponent;
	Action mCurrentAction;
	// Direction variable to determine whether to flip the Texture/Sprite
	// int mCharDir;
};

#endif	// _CHARACTER_HPP