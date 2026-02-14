#pragma once
#include <fstream>
#include "surface.h"
#include "Graphics.h"
class Maze
{
private:
	enum class Tile
	{
		floor,
		wall,
		entrance,
		exit,
		cheese
	};
public:
    Maze()
    {
        std::ifstream file("Files/Data/maze.txt");

        if (!file.is_open())
        {
            return;
        }

        std::string line;
        int row = 0;

        while (std::getline(file, line) && row < nY_Tiles)
        {
            for (int col = 0; col < nX_Tiles && col < (int)line.length(); ++col)
            {
                int index = row * nX_Tiles + col;

                switch (line[col])
                {
                case 'W': tiles[index] = Tile::wall;     break;
                case 'E': tiles[index] = Tile::entrance; break;
                case 'X': tiles[index] = Tile::exit;     break;
                case 'C': tiles[index] = Tile::cheese;   break;
                case 'F':
                default:  tiles[index] = Tile::floor;    break;
                }
            }
            row++;
        }
        file.close();
    }
    void Draw(Graphics& gfx) const
    {

    }
private:
	static constexpr int nX_Tiles = 20;
	static constexpr int nY_Tiles = 15;
	static constexpr int tileSize = 40;
	Tile tiles[nX_Tiles * nY_Tiles];

	Surface spriteCheese = Surface{ "Files/Images/Sprites/cheese.bmp" };
	Surface spriteWall = Surface{ "Files/Images/Sprites/wall.bmp" };
	Surface spriteFloor = Surface{ "Files/Images/Sprites/floor.bmp" };
};