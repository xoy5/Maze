#pragma once
#include <string>
#include <utility>

#include "Graphics.h"
#include "Vec2.h"
#include "Rect.h"

#include "MazeCharacter.h"
#include "Maze.h"

class Player : public MazeCharacter
{
public:
	Player(const Maze& maze, const std::string& spriteFilePath, float speed, int width, int height, int nFrames, float frameHoldTime, bool animationPingPong)
		:
		MazeCharacter(maze, maze.GetEntranceTilePos(), spriteFilePath, speed, width, height, nFrames, frameHoldTime, animationPingPong, true)
	{}
	void ResetToDefault(const Maze& maze) override
	{
		sprintMode = false;
		MazeCharacter::ResetToDefault(maze);
	}
	void Update(float dt, const Maze& maze) override
	{
		MazeCharacter::SetSpeed(GetCurrentSpeed());
		MazeCharacter::Update(dt, maze);
	}
	void SetSprintMode(bool active)
	{
		sprintMode = active;
	}
	RectF GetHitboxRect() const
	{
		return GetRect().GetShrunken(15);
	}
private:
	float GetCurrentSpeed() const
	{
		return MazeCharacter::GetDefaultSpeed() * (sprintMode ? 5.0f : 1.0f);
	}

private:
	bool sprintMode = false;
};