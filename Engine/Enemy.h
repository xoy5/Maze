#pragma once
#include <string>
#include <utility>
#include <map>

#include "Graphics.h"
#include "Vec2.h"
#include "Rect.h"

#include "MazeCharacter.h"
#include "Maze.h"

class Enemy : public MazeCharacter
{
public:
	Enemy(const Maze& maze, const MazeCharacter* pTarget, const std::string& spriteFilePath, float speed, int width, int height, int nFrames, float frameHoldTime, bool animationPingPong)
		:
		MazeCharacter(maze, maze.GetExitTilePos(), spriteFilePath, speed, width, height, nFrames, frameHoldTime, animationPingPong, false),
		pTarget(pTarget)
	{}
    void ResetToDefault(const Maze& maze) override
    {
        isTargetCaught = false;
        MazeCharacter::ResetToDefault(maze);
    }
    virtual void Update(float dt, const Maze& maze)
    {
        CalculateAndSetDirection(maze);
        MazeCharacter::Update(dt, maze);
        if (MazeCharacter::GetHitboxRect().IsOverlappingWith(pTarget->GetHitboxRect()))
        {
            isTargetCaught = true;
        }
    }
	void SetTarget(const MazeCharacter* pTarget_in)
	{
		pTarget = pTarget_in;
	}
    bool IsTargetCaught() const
    {
        return isTargetCaught;
    }
    RectF GetHitboxRect() const
    {
        return GetRect().GetShrunken(10);
    }
    int GetAlgorithmRunCount() const
    {
        return algorithmRunCount;
    }
private:
    void CalculateAndSetDirection(const Maze& maze)
    {
        if (movement.IsMovedToNextTile(maze, *this) || movement.IsMoving() == false || movement.IsNextMoveReserved() == false)
        {
            if (movement.IsMoving() == false)
            {
                algorithmRunCount++;
                const auto currentPosTile = GetTilePos();
                const auto targetPosTile = pTarget->GetTilePos();

                const auto nextPosTile = CalculateBFS(maze, currentPosTile, targetPosTile);

                const Vec2 dir = Movement::GetDirectionFromTiles(currentPosTile, nextPosTile);
                MazeCharacter::SetMovementDirection(dir, maze);
            }
            if (movement.IsNextMoveReserved() == false)
            {
                algorithmRunCount++;
                const auto nextPosTile = GetNextTilePos();
                const auto targetPosTile = pTarget->GetTilePos();

                const auto nextnextPosTile = CalculateBFS(maze, nextPosTile, targetPosTile);

                const Vec2 dir = Movement::GetDirectionFromTiles(nextPosTile, nextnextPosTile);
                MazeCharacter::SetMovementDirection(dir, maze);
            }
        }
    }
	std::pair<int, int> CalculateBFS(const Maze& maze, std::pair<int, int> startPosTile, std::pair<int, int> targetPosTile)
	{
		if (startPosTile == targetPosTile) return startPosTile;

		std::queue<std::pair<int, int>> q;
		q.push(startPosTile);

		std::map<std::pair<int, int>, std::pair<int, int>> parent;
		parent[startPosTile] = startPosTile;

		const std::vector<std::pair<int, int>> directions = { {1,0}, {-1,0}, {0,1}, {0,-1} };

        bool found = false;
        while (!q.empty())
        {
            std::pair<int, int> current = q.front();
            q.pop();

            if (current == targetPosTile)
            {
                found = true;
                break;
            }

            for (const auto& d : directions)
            {
                std::pair<int, int> next = { current.first + d.first, current.second + d.second };

                if (maze.CanEnter(next) && parent.find(next) == parent.end())
                {
                    parent[next] = current;
                    q.push(next);
                }
            }
        }

        if (found)
        {
            std::pair<int, int> step = targetPosTile;

            while (parent[step] != startPosTile)
            {
                step = parent[step];
            }
            return step;
        }

        return startPosTile;
	}

private:
	const MazeCharacter* pTarget = nullptr;
    bool isTargetCaught = false;
    int algorithmRunCount = 0;
};