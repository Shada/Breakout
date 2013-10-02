#include "Water.h"


Water::Water(float _waterLevel)
{
	waterLevel = _waterLevel;
	paused = false;
	maxSpeed = 1.f;
	acceleration = 0.1f;
	speed = .4f;
	speedModifier = 1.0f;
	pausedDuration = 0;
	pauseFlow(5.f);
}

void Water::update(double _dt)
{
	// if paused only tick the timer and dont move the waterlevel
	if(paused)
	{
		pausedDuration -= _dt;
		if(pausedDuration < 0)
		{
			paused = false;
			pausedDuration = 0;
		}
	}
	else
	{
		if(speedDuration > 0)
		{
			speedDuration -= _dt;
			if(speedDuration < 0)
			{
				speedModifier = 1;
				speedDuration = 0;
			}
		}
		speed += acceleration * (float)_dt;
		if(speed > maxSpeed)
		{
			speed = maxSpeed;
		}
		waterLevel += speed * speedModifier * (float)_dt;
	}
}
void Water::setSpeedModifier(float _modifier, double _duration)
{
	speedModifier = _modifier;
	speedDuration = _duration;
}
void Water::reverseFlow()
{
	speed = maxSpeed;
	speed *= -1;
}
void Water::pauseFlow(double _duration)
{
	paused = true;
	pausedDuration += _duration;
}
Water::~Water()
{
}