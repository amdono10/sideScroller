#ifndef _GAME_HPP
#define _GAME_HPP

#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>

//struct to represent and store <x,y> vector coordinates
//struct Vector2 {
	//float x;
	//float y;
//}

class Game {
public:
	// constructor	
	Game();
	// game intialization
	bool Initialize();
	// run and execute the game loop
	void RunLoop();
	// safely shutdown the game
	void Shutdown();

	void AddEntity(class Entity* entity);
	void RemoveEntity(class Entity* entity);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);
private:
	// functions for game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// Map of textures loaded
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// All the entities in the game
	std::vector<class Entity*> mEntities;
	// Any pending entities
	std::vector<class Entity*> mPendingEntities;

	// all of the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;

	// private members of the game
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	uint32_t mTicksCount;

	// game running status
	bool mIsRunning;
	// track if we're updating entities right now
	bool mUpdatingEntities;

	// Game-specific
	class Ship* mShip;	// player's ship
};


#endif // _GAME_HPP