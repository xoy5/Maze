#pragma once
#include <fstream>
#include <assert.h>

#include "Surface.h"
#include "SpriteEffect.h"
#include "Graphics.h"
#include "Rect.h"

class Maze
{
private:
	// Tile definitions using bitwise flags.
	// NOTE: Each tile should be assigned exactly ONE type.
	// Logic uses bitwise values to simplify collision checking
	enum class Tile
	{
		Undefined = 0,
		Floor = 1,
		Entrance = 2,
		Exit = 4,
		Cheese = 8,
		Wall = 16
	};

public:
	Maze()
	{
		SetupTiles();
	}
	void ResetToDefault()
	{
		SetupTiles();
	}
	void Draw(Graphics& gfx) const
	{
		for (int i = 0; i < nTilesX * nTilesY; i++)
		{
			const int x = i % nTilesX;
			const int y = i / nTilesX;
			const Vec2 posOfTile = GetPosOfTileAt({ x,y });

			switch (tiles[i])
			{
			case Tile::Wall:
				gfx.DrawSprite(posOfTile.x, posOfTile.y, spriteWall, SpriteEffect::Copy{});
				break;
			case Tile::Cheese:
				gfx.DrawSprite(posOfTile.x, posOfTile.y, spriteFloor, SpriteEffect::Copy{});
				gfx.DrawSprite(posOfTile.x, posOfTile.y, spriteCheese, SpriteEffect::Chroma{ Colors::Magenta });
				break;
			case Tile::Floor:
				gfx.DrawSprite(posOfTile.x, posOfTile.y, spriteFloor, SpriteEffect::Copy{});
				break;
			case Tile::Entrance:
				gfx.DrawSprite(posOfTile.x, posOfTile.y, spriteFloor, SpriteEffect::Copy{});
				gfx.DrawTransparentRect(GetRectOfTileAt({ x,y }), Colors::Green);
				break;
			case Tile::Exit:
				gfx.DrawSprite(posOfTile.x, posOfTile.y, spriteFloor, SpriteEffect::Copy{});
				gfx.DrawTransparentRect(GetRectOfTileAt({ x,y }), Colors::Red);
				break;
			}
		}
	}
	void DrawTileHighlightAt(Graphics& gfx, std::pair<int, int>tilePos, const Color& c) const
	{
		const Vec2 posOfTile = GetPosOfTileAt(tilePos);
		gfx.DrawTransparentRect(RectF{ posOfTile, float(tileSize), float(tileSize) }, c);
	}
	bool CheckAndCollectCheese(std::pair<int, int> playerTilePos)
	{
		if (int(GetTileAt(playerTilePos)) & int(Tile::Cheese))
		{
			SetTileAt(playerTilePos, Tile::Floor);
			return true;
		}
		return false;
	}
public:
	bool CanEnter(std::pair<int, int> tilePos) const
	{
		return bool(int(GetTileAt(tilePos)) & 15); // 15 means that I choose Floor, Entrance, Exit and Cheese
	}
	std::pair<int, int> GetExitTilePos() const
	{
		for (int i = 0; i < nTilesX * nTilesY; i++)
		{
			if (int(tiles[i]) & int(Tile::Exit))
				return { i % nTilesX, i / nTilesX };
		}

		assert(false && "There is no exit.");
		return {-1,-1};
	}
	std::pair<int, int> GetEntranceTilePos() const
	{
		for (int i = 0; i < nTilesX * nTilesY; i++)
		{
			if (int(tiles[i]) & int(Tile::Entrance))
				return { i % nTilesX, i / nTilesX };
		}

		assert(false && "There is no entrance.");
		return {-1,-1};
	}
	Vec2 GetEntrancePos() const
	{
		return GetPosOfTileAt(GetEntranceTilePos());
	}
	Vec2 GetPosOfTileAt(std::pair<int, int> tilePos) const
	{
		return Vec2{ float(tilePos.first * tileSize), float(tilePos.second * tileSize) };
	}
	RectF GetRectOfTileAt(std::pair<int, int> tilePos) const
	{
		return RectF(Vec2{ float(tilePos.first * tileSize), float(tilePos.second * tileSize) }, float(tileSize), float(tileSize));
	}
	int GetNumberOfCheeses() const
	{
		int nOfCheese = 0;
		for (int i = 0; i < nTilesX * nTilesY; i++)
		{
			if (int(tiles[i]) & int(Tile::Cheese)) nOfCheese++;
		}
		return nOfCheese;
	}

	int GetNumberOfTilesX() const
	{
		return nTilesX;
	};
	int GetNumberOfTilesY() const
	{
		return nTilesY;
	};
	int GetTileSize() const
	{
		return tileSize;
	}

private:
	void SetupTiles()
	{
		std::ifstream file("Files/Data/maze.txt");
		assert(file && "File Error");

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
	Tile GetTileAt(std::pair<int, int> tilePos) const
	{
		if (!(tilePos.first >= 0 && tilePos.first < nTilesX &&
			tilePos.second >= 0 && tilePos.second < nTilesY))
		{
			return Tile::Undefined;
		}

		return tiles[tilePos.first + tilePos.second * nTilesX];
	}
	void SetTileAt(std::pair<int, int> tilePos, Tile type)
	{
		tiles[tilePos.second * nTilesX + tilePos.first] = type;
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