#ifndef _SHIP_HPP
#define _SHIP_HPP

#include "entity.hpp"

class Ship : public Entity
{
public:
	Ship(class Game* game);
	void UpdateEntity(float deltaTime) override;
	void ProcessKeyboard(const bool* state);
	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }

private:
	float mRightSpeed;
	float mDownSpeed;
};

#endif	// _SHIP_HPP