
#include "character.hpp"
#include "entity.hpp"
#include "animSpriteComponent.hpp"
#include "game.hpp"
#include "math.hpp"

Character::Character(Game* game):
	Entity(game),
	mRightSpeed(0.0f),
	mDownSpeed(0.0f),
	mCurrentAction(Action::None)
{
	// create an Animated Sprite Component
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Character01.png"),
		game->GetTexture("Assets/Character02.png"),
		game->GetTexture("Assets/Character03.png"),
		game->GetTexture("Assets/Character04.png"),
		game->GetTexture("Assets/Character05.png"),
		game->GetTexture("Assets/Character06.png"),
		game->GetTexture("Assets/Character07.png"),
		game->GetTexture("Assets/Character08.png"),
		game->GetTexture("Assets/Character09.png"),
		game->GetTexture("Assets/Character10.png"),
		game->GetTexture("Assets/Character11.png"),
		game->GetTexture("Assets/Character12.png"),
		game->GetTexture("Assets/Character13.png"),
		game->GetTexture("Assets/Character14.png"),
		game->GetTexture("Assets/Character15.png"),
		game->GetTexture("Assets/Character16.png"),
		game->GetTexture("Assets/Character17.png"),
		game->GetTexture("Assets/Character18.png")
	};
	asc->SetAnimTextures(anims);
	
	// set Frame animations
	asc->AddAnimation("Walking", 0, 5, true);
	asc->AddAnimation("Jumping", 6, 14, false);
	asc->AddAnimation("Punch", 15, 17, false);
	
	// Default animation for character = Walking
	asc->SetAnimation("Walking");

	// save pointer to asc in character member mAnimComponent
	mAnimComponent = asc;
}

void Character::UpdateEntity(float deltaTime)
{
	Entity::UpdateEntity(deltaTime);
	// Update position based on speeds and delta time
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;
	// Restrict position to left half of the screen
	if (pos.x < 25.0f)
	{
		pos.x = 25.0f;
	} 
	else if (pos.x > 500.0f)
	{
		pos.x = 500.0f;
	}
	if (pos.y < 25.0f)
	{
		pos.y = 25.0f;
	}
	else if (pos.y > 743.0f)
	{
		pos.y = 743.0f;
	}
	SetPosition(pos);

	// If a jump or punch just finished playing, drop back to walking
	if (mCurrentAction != Action::None && mAnimComponent->IsAnimationFinished())
	{
		mCurrentAction = Action::None;
		mAnimComponent->SetAnimation("Walking");
	}
}

void Character::ProcessKeyboard(const bool* state)
{
	// if no key pressed, speed is 0
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;
	// right/left

	// only start a new action if we're not already mid-jump/mid-punch
	//  Without this guard, holding SPACE would call SetAnimation("Jumping")
	// every frame, which resets mCurrFrame back to frame 0 each time --
	// the animation would never actually advance past its first frame.
	if (mCurrentAction == Action::None)
	{
		// Do I need to update the animation state here?
		if (state[SDL_SCANCODE_SPACE])
		{
			mCurrentAction = Action::Jumping;
			mAnimComponent->SetAnimation("Jumping");	
		}
		else if (state[SDL_SCANCODE_P])
		{
			mCurrentAction = Action::Punching;
			mAnimComponent->SetAnimation("Punching");
		}
	}
	if (state[SDL_SCANCODE_D])
	{
		mRightSpeed += 250.0f;
	}
	if (state[SDL_SCANCODE_A])
	{
		mRightSpeed -= 250.0f;
	}
	// up/down
	if (state[SDL_SCANCODE_S])
	{
		mDownSpeed += 300.0f;
	}
	if (state[SDL_SCANCODE_W])
	{
		mDownSpeed -= 300.0f;
	}
	
}