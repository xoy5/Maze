#include "Movement.h"
#include "MazeCharacter.h"

Movement::Movement(const Maze& maze, MazeCharacter& owner)
	:
	tilePos(maze.GetEntranceTilePos())
{
	owner.SetPos(maze.GetEntrancePos());
	SetStandingDirectionBasedOnMaze(maze, owner);
}

void Movement::ResetToDefault(const Maze& maze, MazeCharacter& owner)
{
	tilePos = maze.GetEntranceTilePos();
	owner.SetPos(maze.GetEntrancePos());
	Movement::SetStandingDirectionBasedOnMaze(maze, owner);
}

void Movement::Update(float dt, const Maze& maze, MazeCharacter& owner)
{
	if (curMove == Move::No) return;

	owner.SetVelocity(owner.GetDirection() * owner.GetSpeed());
	owner.Translate(owner.GetVelocity() * dt);

	if (IsMovedToNextTile(maze, owner))
	{
		tilePos = GetNextTilePos();
		SnapToGrid(maze, owner);

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

		owner.SetDirection(GetVecFromMove(curMove));
	}
}

void Movement::SetDirection(const Vec2& dir, const Maze& maze, MazeCharacter& owner)
{
	Move inputMove = GetMoveFromVec(dir);
	if (inputMove == Move::No) return;


	// Handle instantaneous 180-degree turns
	if (IsOpposite(curMove, inputMove))
	{
		tilePos = GetNextTilePos();
		curMove = inputMove;
		const Vec2 moveDir = GetVecFromMove(curMove);
		owner.SetDirection(moveDir);
		return;
	}

	// Immediate move if standing still
	if (curMove == Move::No)
	{
		if (maze.CanEnter(GetTilePosFromMove(tilePos, inputMove)))
		{
			curMove = inputMove;
			const Vec2 moveDir = GetVecFromMove(curMove);
			owner.SetDirection(moveDir);
			if (inputMove == Move::Down)
			{
				float i = 0.0f;
				owner.Translate(Vec2{ i,i });
			}
		}
	}
	// Queue the move to be processed at the next tile junction
	else
	{
		nextMove = inputMove;
	}
}

void Movement::SetStandingDirectionBasedOnMaze(const Maze& maze, MazeCharacter& owner)
{
	auto tilePos = maze.GetEntranceTilePos();
	if (tilePos.first == 0)									 owner.SetStandingDirection(GetVecFromMove(Move::Right));
	else if (tilePos.first == maze.GetNumberOfTilesX() - 1)  owner.SetStandingDirection(GetVecFromMove(Move::Left));
	else if (tilePos.second == maze.GetNumberOfTilesY() - 1) owner.SetStandingDirection(GetVecFromMove(Move::Up));
	else /* if (tilePos.second == 0) obvious shit */         owner.SetStandingDirection(GetVecFromMove(Move::Down));
}

bool Movement::IsMoving() const
{
	return curMove != Move::No;
}

bool Movement::IsOpposite(Move a, Move b) const
{
	if (a == Move::Left && b == Move::Right) return true;
	if (a == Move::Right && b == Move::Left) return true;
	if (a == Move::Up && b == Move::Down)    return true;
	if (a == Move::Down && b == Move::Up)    return true;

	return false;
}

std::pair<int, int> Movement::GetTilePosFromMove(std::pair<int, int> base, Move m) const
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

Vec2 Movement::GetVecFromMove(Move m) const
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

Movement::Move Movement::GetMoveFromVec(Vec2 v) const
{
	// x axis
	if (v.x < 0.0f && !(curMove == Move::Left && v.y != 0.0f /* Leetcode Kira 210IQ */)) return Move::Left;
	else if (v.x > 0.0f && !(curMove == Move::Right && v.y != 0.0f))				     return Move::Right;
	// y axis
	else if (v.y < 0.0f) return Move::Up;
	else if (v.y > 0.0f) return Move::Down;

	return Move::No;
}

std::pair<int, int> Movement::GetTilePos() const
{
	return tilePos;
}

std::pair<int, int> Movement::GetNextTilePos() const
{
	return GetTilePosFromMove(tilePos, curMove);
}

void Movement::SnapToGrid(const Maze& maze, MazeCharacter& owner)
{
	owner.SetPos(maze.GetPosOfTileAt(tilePos));
}

bool Movement::IsMovedToNextTile(const Maze& maze, MazeCharacter& owner)
{
	// return Character::GetRect().IsContainedBy(maze.GetRectOfTileAt(tilePos).GetExpanded(5.0f)); I am getting smarter and smarter 
	const RectF ownerRect = owner.GetRect();
	const RectF nextTileRect = maze.GetRectOfTileAt(GetNextTilePos());

	switch (curMove)
	{
	case Move::Left:
		return ownerRect.right <= nextTileRect.right;
	case Move::Right:
		return ownerRect.left >= nextTileRect.left;
	case Move::Up:
		return ownerRect.bottom <= nextTileRect.bottom;
	case Move::Down:
		return ownerRect.top >= nextTileRect.top;
	case Move::No:
		return true;
	}
}