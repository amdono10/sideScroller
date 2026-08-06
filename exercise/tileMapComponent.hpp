
#ifndef _TILE_MAP_COMPONENT
#define _TILE_MAP_COMPONENT

#include <vector>
#include <string>

#include "spriteComponent.hpp"


class TileMapComponent : public SpriteComponent
{
public:
	// constructor w/ default drawOrder of 10
	// reminder: lower draw order -> further in the background
	// the painter algorithm draws ONTOP of previously drawn textures
	TileMapComponent(class Entity* owner, int drawOrder = 10, const std::string& mapFileName = nullptr);
	// update and draw overridden from SpriteComponent
	void Update(float deltaTime) override;
	// draw needs to loop through the values provided in the tilemap CSV file
	void Draw(SDL_Renderer* renderer) override;
	// SpriteComponent has its own SetTexture function, one is not needed here
	// Setter for mFileName = MapLayer CSV file, need to call this in Game::LoadData()
	// void SetFileName(const std::string& filename) { mFileName = filename; }

	// LoadTileIDMatrix should only be called once, i.e. in Game::LoadData()
	void LoadTileIDMatrix();
 
private:
	// data type to hold the values from the TileMap CSV
	std::vector<std::vector<int>> mTileIDs;
	std::string mFileName;

	const int height;
	const int width;
};


#endif	// _TILE_MAP_COMPONENT