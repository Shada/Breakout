#include "Water.h"


Water::Water(float _waterLevel)
{
	waterLevel = _waterLevel;
	paused = false;
	pauseFlow(5.f);
	maxSpeed = 10.f;
	speed = 4.f;
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