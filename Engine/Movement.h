#pragma once
#include <string>
#include <utility>

#include "Graphics.h"
#include "Vec2.h"
#include "Rect.h"

#include "Maze.h"

class MazeCharacter;

class Movement
{
private:
	enum class Move
	{
		Left,
		Right,
		Up,
		Down,
		No
	};

public:
	Movement(const Maze& maze, MazeCharacter& owner);
	void ResetToDefault(const Maze& maze, MazeCharacter& owner);
	void Update(float dt, const Maze& maze, MazeCharacter& owner);
	void SetDirection(const Vec2& dir, const Maze& maze, MazeCharacter& owner);
	void SetStandingDirectionBasedOnMaze(const Maze& maze, MazeCharacter& owner);

public:
	std::pair<int, int> GetTilePos() const;
	std::pair<int, int> GetNextTilePos() const;

public:
	void SnapToGrid(const Maze& maze, MazeCharacter& owner);
	bool IsMovedToNextTile(const Maze& maze, MazeCharacter& owner);

	bool IsMoving() const;
	bool IsOpposite(Move a, Move b) const;
	Vec2 GetVecFromMove(Move m) const;
	Move GetMoveFromVec(Vec2 v) const;
	std::pair<int, int> GetTilePosFromMove(std::pair<int, int> base, Move m) const;

private:
	Move curMove = Move::No;
	Move nextMove = Move::No;
	std::pair<int, int> tilePos;
};