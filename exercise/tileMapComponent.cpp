
#include "tileMapComponent.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

TileMapComponent::TileMapComponent(class Entity* owner, int drawOrder, const std::string& mapFileName):
	SpriteComponent(owner, drawOrder),
	height(32),
	width(32)
{	
	// mTileIDs = new std::vector<std::vector<int>>;
	mFileName = mapFileName;
}

void TileMapComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
	// I think thats all that goes here	
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	// need to calculate srcrct and dstrct -> what SDL_FRect is the source in the texture (Tiles.png in this case)
	// what SDL_FRect is the dest i.e. where on the screen will the srcrct be drawn
	if(mFileName.empty())
	{
		// If we have not set a source file for the map
		SDL_Log("TileMapComponent was not properly constructed with a mapFileName\n");
		return;
	}
	if(mTileIDs.empty())
	{
		SDL_Log("TileID Matrix was not loaded. Call TileMapComponent::LoadTileIDMatrix() in Game::LoadData()\n");
		return;
	}
	else
	{
		// 32x32 Tiles (both source and dest)
		SDL_FRect source;
		source.w = width;
		source.h = height;
		source.x = 0;
		source.y = 0;
		SDL_FRect dest;
		dest.w = width;
		dest.h = height;
		dest.x = 0;
		dest.y = 0;
		// iterate mTileIDs matrix to get the value
		for(int row = 0; row < mTileIDs.size(); ++row)
		{
			for(int col = 0; col <  mTileIDs[row].size();  ++col)
			{
				int val = mTileIDs[row][col];
				if (val >= 0)
				{
					// if val != -1, it is a valid source tile
					// now calculate the source tile
					// here we use 8 because the tile map has 8 tiles per row
					// in other words, Tiles.png is 8 x 24 (w x h) in tiles
					source.x = static_cast<int>(val % 8) * width;
					source.y = static_cast<int>(val / 8) * height;	
					
					// SDL_RenderTexture(SDL_renderer* renderer, SDL_Texture* texture, const SDL_FRect * srcrct, const SDL_FRect* dstrct)
					SDL_RenderTexture(renderer,
						mTexture,
						&source,
						&dest);
				}
				dest.x += width;
			}
			dest.y += height;
			// reset x coordinate back to 0 
			dest.x = 0;
		}
	}
}

void TileMapComponent::LoadTileIDMatrix()
{
	std::ifstream file(mFileName);
	std::string line;

	while (std::getline(file, line))
	{
		std::vector<int> row;
		std::stringstream ss(line);
		std::string val;

		while (std::getline(ss, val, ','))
		{
			row.push_back(std::stoi(val));
		}
		mTileIDs.push_back(row);
	}
}