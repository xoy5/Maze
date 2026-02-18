#pragma once
#include <string>
#include <utility>

#include "Graphics.h"
#include "Vec2.h"
#include "Rect.h"

#include "Character.h"
#include "Maze.h"

class PlayerMouse : private Character
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
	PlayerMouse(const std::string& spriteFilePath, Vec2 pos, float speed, int width, int height, int nFrames, float frameHoldTime, bool animationPingPong, std::pair<int, int> tilePos)
		:
		Character(spriteFilePath, pos, speed, width, height, nFrames, frameHoldTime, animationPingPong),
		tilePos(tilePos)
	{}
	void Draw(Graphics& gfx) const
	{
		Character::Draw(gfx);
	}
	void Update(float dt, const Maze& maze)
	{
		if (curMove == Move::No) return;
		
		Character::Update(dt);

		if (IsInTile(maze, GetNextTilePos()))
		{
			tilePos = GetNextTilePos();
			SnapToGrid(maze);

			// if can turn
			if (nextMove != Move::No && maze.CanEnter(GetTilePosFromMove(tilePos, nextMove)))
			{
				curMove = nextMove;
				nextMove = Move::No;
			}
			// if cannot go forward stop
			else if (!maze.CanEnter(GetTilePosFromMove(tilePos, curMove)))
			{
				curMove = Move::No;
			}
			Character::SetDirection(GetVecFromMove(curMove));
		}
	}
	void SetDir(const Vec2& dir, const Maze& maze)
	{
		Move inputMove = GetMoveFromVec(dir);
		if (inputMove == Move::No) return;

		// TO DO 180 degree rotation

		if (curMove == Move::No)
		{
			if (maze.CanEnter(GetTilePosFromMove(tilePos, inputMove)))
			{
				curMove = inputMove;
				Character::SetDirection(GetVecFromMove(curMove));
			}
		}
		else
		{
			nextMove = inputMove; // gonna check in update if can move
		}

	}
	void SetStandingDir(const Maze& maze)
	{
		auto tilePos = maze.GetEntranceTilePos();
		if (tilePos.first == 0) Character::SetStandingDirection(GetVecFromMove(Move::Right));
		else if (tilePos.first == maze.GetNumberOfTilesX() - 1) Character::SetStandingDirection(GetVecFromMove(Move::Right));
		else if (tilePos.second == maze.GetNumberOfTilesY() - 1) Character::SetStandingDirection(GetVecFromMove(Move::Up));
		else if (tilePos.second == 0) Character::SetStandingDirection(GetVecFromMove(Move::Down));
	}

public:
	std::pair<int, int> GetTilePos() const
	{
		return tilePos;
	}
	std::pair<int, int> GetNextTilePos() const
	{
		return GetTilePosFromMove(tilePos, curMove);
	}

private:
	bool IsMoving() const
	{
		return curMove != Move::No;
	}
	bool IsOpposite(Move a, Move b) const
	{
		if (a == Move::Left && b == Move::Right) return true;
		if (a == Move::Right && b == Move::Left) return true;
		if (a == Move::Up && b == Move::Down) return true;
		if (a == Move::Down && b == Move::Up) return true;
		return false;
	}
	std::pair<int, int> GetTilePosFromMove(std::pair<int, int> base, Move m) const
	{
		switch (m)
		{
		case Move::Left:  return { base.first - 1, base.second };
		case Move::Right: return { base.first + 1, base.second };
		case Move::Up:    return { base.first, base.second - 1 };
		case Move::Down:  return { base.first, base.second + 1 };
		default:          return base;
		}
	}
	Vec2 GetVecFromMove(Move m) const
	{
		switch (m)
		{
		case Move::Left:  return { -1.0f,  0.0f };
		case Move::Right: return { 1.0f,  0.0f };
		case Move::Up:    return { 0.0f, -1.0f };
		case Move::Down:  return { 0.0f,  1.0f };
		default:          return { 0.0f,  0.0f };
		}
	}
	Move GetMoveFromVec(Vec2 v) const
	{
		if (v.x < 0.0f && !(curMove == Move::Left && v.y != 0.0f /*Leetcode Kira 210IQ*/)) return Move::Left;
		else if (v.x > 0.0f && !(curMove == Move::Right && v.y != 0.0f)) return Move::Right;
		else if (v.y < 0.0f) return Move::Up;
		else if (v.y > 0.0f) return Move::Down;
		return Move::No;
	}
	void SnapToGrid(const Maze& maze)
	{
		Character::SetPos(maze.GetPosOfTileAt(tilePos));
	}
	bool IsInTile(const Maze& maze, std::pair<int, int> tilePos)
	{
		return Character::GetRect().IsContainedBy(maze.GetRectOfTileAt(tilePos).GetExpanded(5.0f));
	}

private:
	Move curMove = Move::No;
	Move nextMove = Move::No;
	std::pair<int, int> tilePos;
};