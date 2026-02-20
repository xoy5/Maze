#pragma once
#include <string.h>

#include "Animation.h"
#include "Vec2.h"
#include "Movement.h"
#include "Maze.h"

class MazeCharacter
{
	friend class Movement;

private:
	enum class Sequence
	{
		WalkingLeft,
		WalkingRight,
		WalkingUp,
		WalkingDown,
		StandingLeft,
		StandingRight,
		StandingUp,
		StandingDown,
		Count
	};

public:
	MazeCharacter(const Maze& maze, const std::pair<int, int>& spawnPoint, const std::string& spriteFilePath, float speed, int width, int height, int nFrames, float frameHoldTime, bool animationPingPong = false, bool canTurnImmediately = false);
	virtual void ResetToDefault(const Maze& maze);
	void Draw(Graphics& gfx) const;
	virtual void Update(float dt, const Maze& maze);
	void SetAnimationDirection(Vec2 dir);
	// activates a damage visual effect
	void ActivateEffect();

public:
	Vec2 GetDirection() const;
	void SetMovementDirection(const Vec2& dir, const Maze& maze);
	void SetStandingDirection(const Vec2& dir);

	void SetPos(const Vec2& pos_in);
	Vec2 GetPos() const;
	std::pair<int, int> GetTilePos() const;
	std::pair<int, int> GetNextTilePos() const;
	void Translate(const Vec2& translate);
	bool CanTurnImmediately() const;

	void SetSpeed(float speed_in);
	float GetSpeed() const;
	float GetDefaultSpeed() const;

	void SetVelocity(Vec2 velocity_in);
	Vec2 GetVelocity() const;

	RectF GetRect() const;
	virtual RectF GetHitboxRect() const;
	int GetWidth() const;
	int GetHeight() const;

private:
	void SetDirection(const Vec2& dir_in);

private:
	Surface sprite;
	int width;
	int height;
	Vec2 pos = { 0.0f, 0.0f };
	Vec2 vel = { 0.0f, 0.0f };
	Vec2 dir = { 0.0f, 0.0f };
	std::vector<Animation> animations;
	Sequence iCurSequence = Sequence::StandingDown;
	float speed;
	const float defaultSpeed;
	static constexpr float effectDuration = 0.045f;
	float effectTime = 0.0f;
	bool effectActive = false;
	bool canTurnImmediately;

protected:
	Movement movement;
};