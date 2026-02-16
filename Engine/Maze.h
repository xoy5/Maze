#pragma once
#include <fstream>
#include <assert.h>

#include "Surface.h"
#include "SpriteEffect.h"
#include "Graphics.h"
#include "Rect.h"

class Maze
{
public:
	enum class Tile
	{
		Floor = 1,
		Entrance = 2,
		Exit = 4,
		Cheese = 8,
		Wall = 16
	};

public:
	Maze()
	{
		for (int i = 0; i < nTilesX * nTilesY; i++)
		{
			tiles[i] = Tile::Wall;
		}

		std::ifstream file("Files/Data/maze.txt");
		assert(file, "File Error");

		char c;
		int i = 0;
		while (file.get(c))
		{
			switch (c)
			{
			case '\n':
				i--;
				break;
			case 'W':
				tiles[i] = Tile::Wall;
				break;
			case 'C':
				tiles[i] = Tile::Cheese;
				break;
			case 'F':
				tiles[i] = Tile::Floor;
				break;
			case 'E':
				tiles[i] = Tile::Entrance;
				break;
			case 'X':
				tiles[i] = Tile::Exit;
				break;
			}
			i++;
		}
	}
	void Draw(Graphics& gfx) const
	{
		for (int i = 0; i < nTilesX * nTilesY; i++)
		{
			switch (tiles[i])
			{
			case Tile::Wall:
				gfx.DrawSprite((i % nTilesX) * tileSize, (i / nTilesX) * tileSize, spriteWall, SpriteEffect::Copy{});
				break;
			case Tile::Cheese:
				gfx.DrawSprite((i % nTilesX) * tileSize, (i / nTilesX) * tileSize, spriteFloor, SpriteEffect::Copy{});
				gfx.DrawSprite((i % nTilesX) * tileSize, (i / nTilesX) * tileSize, spriteCheese, SpriteEffect::Chroma{ Colors::Magenta });
				break;
			case Tile::Floor:
				gfx.DrawSprite((i % nTilesX) * tileSize, (i / nTilesX) * tileSize, spriteFloor, SpriteEffect::Copy{});
				break;
			case Tile::Entrance:
				gfx.DrawSprite((i % nTilesX) * tileSize, (i / nTilesX) * tileSize, spriteFloor, SpriteEffect::Substitution{ Colors::Magenta, Colors::Green });
				gfx.DrawSprite((i % nTilesX) * tileSize, (i / nTilesX) * tileSize, spriteFloor, SpriteEffect::Ghost{ Colors::Magenta });
				break;
			case Tile::Exit:
				gfx.DrawSprite((i % nTilesX) * tileSize, (i / nTilesX) * tileSize, spriteFloor, SpriteEffect::Substitution{ Colors::Magenta, Colors::Red });
				gfx.DrawSprite((i % nTilesX) * tileSize, (i / nTilesX) * tileSize, spriteFloor, SpriteEffect::Ghost{ Colors::Magenta });
				break;
			}
		}
	}

public:
	std::pair<int, int> GetEntranceTilePos() const
	{
		for (int i = 0; i < nTilesX * nTilesY; i++)
		{
			if (tiles[i] == Tile::Entrance)
				return { i % nTilesX, i / nTilesX };
		}

		assert(false, "There is no entrance.");
	}
	Vec2 GetEntrancePos() const
	{
		return GetTilePosAt(GetEntranceTilePos());
	}
	Tile GetTileAt(std::pair<int, int> tilePos) const
	{
		return tiles[tilePos.first + tilePos.second * nTilesX];
	}
	bool CanEnter(std::pair<int, int> tilePos) const
	{
		return bool(int(GetTileAt(tilePos)) & 15); // 15 means that I choose Floor, Entrance, Exit and Cheese
	}
	RectF GetTileRectAt(std::pair<int, int> tilePos) const
	{
		return RectF(Vec2{ float(tilePos.first * tileSize), float(tilePos.second * tileSize) }, float(tileSize), float(tileSize));
	}
	Vec2 GetTilePosAt(std::pair<int, int> tilePos) const
	{
		return Vec2{ float(tilePos.first * tileSize), float(tilePos.second * tileSize) };
	}

private:
	static constexpr int nTilesX = 20;
	static constexpr int nTilesY = 15;
	static constexpr int tileSize = 40;
	Tile tiles[nTilesX * nTilesY];

	Surface spriteCheese = Surface{ "Files/Images/Sprites/cheese.bmp" };
	Surface spriteWall = Surface{ "Files/Images/Sprites/wall.bmp" };
	Surface spriteFloor = Surface{ "Files/Images/Sprites/floor.bmp" };
};