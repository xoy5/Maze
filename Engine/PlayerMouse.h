#pragma once
#include <string>

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
	PlayerMouse(const std::string& spriteFilePath, Vec2 pos, int width, int height, int nFrames, float frameHoldTime, bool animationPingPong, std::pair<int, int> tilePos)
		:
		Character(spriteFilePath, pos, width, height, nFrames, frameHoldTime, animationPingPong),
		tilePos(tilePos)
	{
	}
	void Draw(Graphics& gfx) const
	{
		Character::Draw(gfx);
	}
	void Update(float dt, const Maze& maze)
	{
		RectF playerRect = RectF(Character::GetPos(), float(Character::GetWidth()), float(Character::GetHeight()));
		RectF nextMazeTileRectExpanded = maze.GetTileRectAt(GetNextTilePos()).GetExpanded(5.00f);

		if (playerRect.IsContainedBy(nextMazeTileRectExpanded))
		{
			tilePos = GetNextTilePos();
			Character::SetPos(maze.GetTilePosAt(tilePos));
			curMove = nextMove;
			nextMove = Move::No;
		}
		else
		{
 			Character::Update(dt);
		}
	}

public:
	void SetDir(const Vec2& dir, const Maze& maze)
	{
		Move move = Move::No;
		if (dir != Vec2{ 0.0f, 0.0f })
		{
			const auto playerTilePos = IsMoving() ? GetNextTilePos() : GetTilePos();
			if (dir.x != 0.0f && maze.CanEnter({ playerTilePos.first + int(dir.x), playerTilePos.second }))
			{
				if (dir.x < 0.0f) move = Move::Left;
				else if (dir.x > 0.0f) move = Move::Right;
			}
			else if (dir.y != 0.0f && maze.CanEnter({ playerTilePos.first, playerTilePos.second + int(dir.y) }))
			{
				if (dir.y < 0.0f) move = Move::Up;
				else if (dir.y > 0.0f) move = Move::Down;
			}
		}

		if (IsMoving())	
			nextMove = move;
		else			
			curMove = move;

		// Setting cur dir
		Vec2 curDir = { 0.0f, 0.0f };

		switch (curMove)
		{
		case Move::Left:
			curDir = { -1.0f, 0.0f };
			break;
		case Move::Right:
			curDir = { 1.0f, 0.0f };
			break;
		case Move::Up:
			curDir = { 0.0f, -1.0f };
			break;
		case Move::Down:
			curDir = { 0.0f, 1.0f };
			break;
		}

		Character::SetDirection(curDir);
	}
	std::pair<int, int> GetTilePos() const
	{
		return tilePos;
	}
	std::pair<int, int> GetNextTilePos() const
	{
		int x = 0;
		int y = 0;

		switch (curMove)
		{
		case Move::Left:
			x--;
			break;
		case Move::Right:
			x++;
			break;
		case Move::Up:
			y--;
			break;
		case Move::Down:
			y++;
			break;
		case Move::No:
			return { -1, -1 };
		}

		return { tilePos.first + x, tilePos.second + y };
	}
	bool IsMoving() const
	{
		return curMove != Move::No;
	}

private:
	Move curMove = Move::No;
	Move nextMove = Move::No;
	std::pair<int, int> tilePos;
};