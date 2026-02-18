#include "Character.h"

Character::Character(const std::string& spriteFilePath, const Vec2& pos, float speed, int width, int height, int nFrames, float frameHoldTime, bool animationPingPong)
	:
	sprite(spriteFilePath),
	pos(pos),
	speed(speed),
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

void Character::Draw(Graphics& gfx) const
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

void Character::Update(float dt)
{
	pos += vel * dt;
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

void Character::ActivateEffect()
{
	effectActive = true;
	effectTime = 0.0f;
}

void Character::SetDirection(const Vec2& dir)
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
	vel = dir * speed;
}

void Character::SetStandingDirection(const Vec2& dir)
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

void Character::SetPos(const Vec2& pos_in)
{
	pos = pos_in;
}

Vec2 Character::GetPos() const
{
	return pos;
}

int Character::GetWidth() const
{
	return width;
}

int Character::GetHeight() const
{
	return height;
}

RectF Character::GetRect() const
{
	return RectF{ pos, float(width), float(height) };
}
