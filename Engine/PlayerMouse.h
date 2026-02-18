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
	PlayerMouse(const Maze& maze, const std::string& spriteFilePath, float speed, int width, int height, int nFrames, float frameHoldTime, bool animationPingPong)
		:
		Character(spriteFilePath, maze.GetEntrancePos(), speed, width, height, nFrames, frameHoldTime, animationPingPong),
		tilePos(maze.GetEntranceTilePos())
	{
		SetStandingDirection(maze);
	}
	void ResetToDefault(const Maze& maze)
	{
		SetPos(maze.GetEntrancePos());
		tilePos = maze.GetEntranceTilePos();
		SetStandingDirection(maze);
	}
	void Draw(Graphics& gfx) const
	{
		Character::Draw(gfx);
	}
	void Update(float dt, const Maze& maze)
	{
		if (curMove == Move::No) return;
		
		Character::SetSpeed(GetCurrentSpeed());
		Character::Update(dt);

		if (IsInTile(maze, GetNextTilePos()))
		{
			tilePos = GetNextTilePos();
			SnapToGrid(maze);

			// Check if the queued next move is valid to perform a turn
			if (nextMove != Move::No && maze.CanEnter(GetTilePosFromMove(tilePos, nextMove)))
			{
				curMove = nextMove;
				nextMove = Move::No;
			}
			// If the current path is blocked, stop the movement
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

		// Handle instantaneous 180-degree turns
		if (IsOpposite(curMove, inputMove))
		{
			tilePos = GetNextTilePos();
			curMove = inputMove;
			Character::SetDirection(GetVecFromMove(curMove));
			return;
		}

		// Immediate move if standing still
		if (curMove == Move::No)
		{
			if (maze.CanEnter(GetTilePosFromMove(tilePos, inputMove)))
			{
				curMove = inputMove;
				Character::SetDirection(GetVecFromMove(curMove));
			}
		}
		// Queue the move to be processed at the next tile junction
		else
		{
			nextMove = inputMove;
		}

	}
	void SetStandingDirection(const Maze& maze)
	{
		auto tilePos = maze.GetEntranceTilePos();
		if (tilePos.first == 0) Character::SetStandingDirection(GetVecFromMove(Move::Right));
		else if (tilePos.first == maze.GetNumberOfTilesX() - 1) Character::SetStandingDirection(GetVecFromMove(Move::Right));
		else if (tilePos.second == maze.GetNumberOfTilesY() - 1) Character::SetStandingDirection(GetVecFromMove(Move::Up));
		else if (tilePos.second == 0) Character::SetStandingDirection(GetVecFromMove(Move::Down));
	}
	void SetSprintMode(bool active)
	{
		sprintMode = active;
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
		// return Character::GetRect().IsContainedBy(maze.GetRectOfTileAt(tilePos).GetExpanded(5.0f)); I am getting smarter and smarter 
		switch (curMove)
		{
		case Move::Left:
			return Character::GetRect().right <= maze.GetRectOfTileAt(GetNextTilePos()).right;
		case Move::Right:
			return Character::GetRect().left >= maze.GetRectOfTileAt(GetNextTilePos()).left;
		case Move::Up:
			return Character::GetRect().bottom <= maze.GetRectOfTileAt(GetNextTilePos()).bottom;
		case Move::Down:
			return Character::GetRect().top >= maze.GetRectOfTileAt(GetNextTilePos()).top;
		case Move::No:
			return true;
		}
	}
	float GetCurrentSpeed() const
	{
		return Character::GetDefaultSpeed() * (sprintMode ? 5.0f : 1.0f);
	}

private:
	Move curMove = Move::No;
	Move nextMove = Move::No;
	std::pair<int, int> tilePos;
	bool sprintMode = false;
};