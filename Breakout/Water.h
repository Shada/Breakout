#pragma once
#include "Resource.h"
class Water
{
private:
	float waterLevel;
	float maxSpeed;
	float speed;
	float acceleration;

	double timer;

	// used for modifier
	float speedModifier;
	double speedDuration;

	// used for pausing the water rising
	bool paused;
	double pausedDuration;

	Vec2 windDirection;
public:
	/* first parameter sets the initial water level.
	   second one: 0 == water, 1 == lava*/
	Water(float _waterLevel, int type);
	~Water();

	float getWaterLevel() { return waterLevel; }

	void setSpeedModifier(float _modifier, double _duration);

	void update(double _dt);
	void reverseFlow();
	void pauseFlow(double _duration);
};


