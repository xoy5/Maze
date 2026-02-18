#pragma once
#include <string.h>

#include "Animation.h"
#include "Vec2.h"

class Character
{
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
	Character(const std::string& spriteFilePath, const Vec2& pos,float speed, int width, int height, int nFrames, float frameHoldTime, bool animationPingPong = false);
	void Draw( Graphics& gfx ) const;
	void Update( float dt );

	// activates a damage visual effect
	void ActivateEffect();

public:
	void SetDirection( const Vec2& dir );
	void SetStandingDirection(const Vec2& dir);
	void SetPos(const Vec2& pos_in);
	Vec2 GetPos() const;
	int GetWidth() const;
	int GetHeight() const;
	RectF GetRect() const;

private:
	Surface sprite;
	int width;
	int height;
	Vec2 pos;
	Vec2 vel = { 0.0f,0.0f };
	std::vector<Animation> animations;
	Sequence iCurSequence = Sequence::StandingDown;
	float speed;
	static constexpr float effectDuration = 0.045f;
	float effectTime = 0.0f;
	bool effectActive = false;
};