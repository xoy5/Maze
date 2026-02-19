#include "MazeCharacter.h"

MazeCharacter::MazeCharacter(const Maze& maze, const std::string& spriteFilePath, float speed, int width, int height, int nFrames, float frameHoldTime, bool animationPingPong)
	:
	movement(maze, *this),
	sprite(spriteFilePath),
	speed(speed),
	defaultSpeed(speed),
	width(width),
	height(height)
{
	for (int i = 0; i < (int)Sequence::StandingLeft; i++)
	{
		animations.emplace_back(Animation(width, height * i, width, height, nFrames, &sprite, frameHoldTime, animationPingPong));
	}
	for (int i = (int)Sequence::StandingLeft; i < (int)Sequence::Count; i++)
	{
		animations.emplace_back(Animation(0, height * (i - (int)Sequence::StandingLeft), width, height, 1, &sprite, frameHoldTime));
	}
}

void MazeCharacter::ResetToDefault(const Maze& maze)
{
	movement.ResetToDefault(maze, *this);
}

void MazeCharacter::Draw(Graphics& gfx) const
{
	// if effect active, draw sprite replacing opaque pixels with red
	if (effectActive)
	{
		animations[(int)iCurSequence].Draw((Vei2)pos, gfx, SpriteEffect::Substitution(Colors::Magenta, Colors::Red));
	}
	else
	{
		animations[(int)iCurSequence].Draw((Vei2)pos, gfx, SpriteEffect::Chroma{ Colors::Magenta });
	}
}

void MazeCharacter::Update(float dt, Maze& maze)
{
	movement.Update(dt, maze, *this);
	animations[(int)iCurSequence].Update(dt);
	// update effect time if active
	if (effectActive)
	{
		effectTime += dt;
		// deactivate effect if duration exceeded
		if (effectTime >= effectDuration)
		{
			effectActive = false;
		}
	}
}

void MazeCharacter::SetAnimationDirection(Vec2 dir)
{
	if (dir.x > 0.0f)
	{
		iCurSequence = Sequence::WalkingRight;
	}
	else if (dir.x < 0.0f)
	{
		iCurSequence = Sequence::WalkingLeft;
	}
	else if (dir.y < 0.0f)
	{
		iCurSequence = Sequence::WalkingUp;
	}
	else if (dir.y > 0.0f)
	{
		iCurSequence = Sequence::WalkingDown;
	}
	else
	{
		if (vel.x > 0.0f)
		{
			iCurSequence = Sequence::StandingRight;
		}
		else if (vel.x < 0.0f)
		{
			iCurSequence = Sequence::StandingLeft;
		}
		else if (vel.y < 0.0f)
		{
			iCurSequence = Sequence::StandingUp;
		}
		else if (vel.y > 0.0f)
		{
			iCurSequence = Sequence::StandingDown;
		}
	}
}

void MazeCharacter::ActivateEffect()
{
	effectActive = true;
	effectTime = 0.0f;
}

void MazeCharacter::SetDirection(const Vec2& dir_in)
{
	dir = dir_in;
	SetAnimationDirection(dir);
}

Vec2 MazeCharacter::GetDirection() const
{
	return dir;
}

void MazeCharacter::SetMovementDirection(const Vec2& dir, const Maze& maze)
{
	movement.SetDirection(dir, maze, *this);
}

void MazeCharacter::SetStandingDirection(const Vec2& dir)
{
	if (dir.x > 0.0f)
	{
		iCurSequence = Sequence::StandingRight;
	}
	else if (dir.x < 0.0f)
	{
		iCurSequence = Sequence::StandingLeft;
	}
	else if (dir.y < 0.0f)
	{
		iCurSequence = Sequence::StandingUp;
	}
	else if (dir.y > 0.0f)
	{
		iCurSequence = Sequence::StandingDown;
	}
}

void MazeCharacter::SetPos(const Vec2& pos_in)
{
	pos = pos_in;
}

Vec2 MazeCharacter::GetPos() const
{
	return pos;
}

std::pair<int, int> MazeCharacter::GetTilePos() const
{
	return movement.GetTilePos();
}

std::pair<int, int> MazeCharacter::GetNextTilePos() const
{
	return movement.GetNextTilePos();
}

void MazeCharacter::Translate(const Vec2& translate)
{
	pos += translate;
}

int MazeCharacter::GetWidth() const
{
	return width;
}

int MazeCharacter::GetHeight() const
{
	return height;
}

RectF MazeCharacter::GetRect() const
{
	return RectF{ pos, float(width), float(height) };
}

float MazeCharacter::GetSpeed() const
{
	return speed;
}

void MazeCharacter::SetSpeed(float speed_in)
{
	speed = speed_in;
}

float MazeCharacter::GetDefaultSpeed() const
{
	return defaultSpeed;
}

void MazeCharacter::SetVelocity(Vec2 velocity_in)
{
	vel = velocity_in;
}

Vec2 MazeCharacter::GetVelocity() const
{
	return vel;
}
