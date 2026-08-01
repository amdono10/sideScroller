#include "game.hpp"
#include <SDL3_image/SDL_image.h>
#include <algorithm>
#include "entity.hpp"
#include "spriteComponent.hpp"
#include "ship.hpp"
#include "bgSpriteComponent.hpp"

// constructor
Game::Game():
mWindow(nullptr),
mRenderer(nullptr),
mTicksCount(0),
mIsRunning(true),
mUpdatingEntities(false)
{

}

bool Game::Initialize()
{
	// initialize SDL
	bool sdlResult = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	if (sdlResult != true){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// create SDL window
	// mWindow = SDL_CreateWindow(
		// "Side Scroller",		// window title
		// 1024,					// window width
		// 768,					// window height
		// SDL_WINDOW_RESIZABLE	// flag to allow resizing of window
	// );

	if (!SDL_CreateWindowAndRenderer("Side Scroller", 1024, 768, SDL_WINDOW_RESIZEABLE, &mWindow, &mRenderer))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
		return false;
	}
	// if(!mWindow){
		// SDL_Log("Failed to create window: %s", SDL_GetError());
		// return false;
	// }

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning){
		// game loop helper functions go here:
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		switch (event.type){
			// if we get an SDL_QUIT event, end loop
			case SDL_EVENT_QUIT:
				mIsRunning = false;
				break;
		}
	}

	// also end loop on ESC key press
	const bool* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]){
		mIsRunning = false;
	}

	// process ship input
	mShip->ProcessKeyboard(state);
}

void Game::UpdateGame()
{
	// compute delta time
	// wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;
	
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// clamp deltaTime	
	if(deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// update all entities
	mUpdatingEntities = true;
	for (auto entity : mEntities)
	{
		entity->Update(deltaTime);
	}
	mUpdatingEntities = false;

	// Move any pending entities to mEntities
	for (auto pending : mPendingEntities)
	{
		mEntities.emplace_back(pending);
	}
	mPendingEntities.clear();

	// Add any dead entities to a temp vector
	std::vector<Entity*> deadEntities;
	for (auto entity : mEntities)
	{
		if (entity->GetState() == Entity::EDead)
		{
			deadEntities.emplace_back(entity);
		}
	}

	// Delete dead entities (which removes them from mEntities)
	for (auto entity : deadEntities)
	{
		delete entity;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	// Draw all sprite components
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	// create player's ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);

	// create entity for the background (this doesn't need a subclass)
	Entity* temp = new Entity(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));

	// create the "far back" background
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);
	
	// Create the closer background
	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);	
}

void Game::UnloadData()
{
	// Delete entities
	// because ~Entity calls RemoveEntity, have to use a different style loop
	while (!mEntities.empty())
	{
		delete mEntities.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// Is the texture already in the map?
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}

	return tex;
}

void Game::Shutdown()
{
	UnloadData();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddEntity(Entity* entity)
{
	// If we're updateing entities, need to add to pending
	if (mUpdatingEntities)
	{
		mPendingEntities.emplace_back(entity);
	}
	else
	{
		mEntities.emplace_back(entity);
	}
}

void Game::RemoveEnity(Entity* entity)
{
	// Is it in pending entities?
	auto iter = std::find(mPendingEntities.begin(), mPendingEntities.end(), entity);
	if (iter != mPendingEntities.end())
	{
		// swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingEntities.end() - 1);
		mPendingEntities.pop_back();
	}

	// Is it in entities?
	iter = std::find(mEntities.begin(), mEntities.end(), entity);
	if (iter != mEntities.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mEntities.end() -1);
		mEntities.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (the first element with ahigher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	for (auto iter = mSprites.begin(); iter != mSprites.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	
	// inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// We can't swap because it ruins ordering
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
