#pragma once
#include "Resource.h"
class Water
{
private:
	float waterLevel;
	float maxSpeed;
	float speed;
	float acceleration;

	//0 == water, 1 == lava
	int type;
	double timer;

	// used for modifier
	float speedModifier;
	double speedDuration;

	// used for pausing the water rising
	bool paused;
	double pausedDuration;

	Vec2 windDirection;
public:
	/* parameter sets the initial water level */
	Water(float _waterLevel, int type);
	~Water();

	float getWaterLevel() { return waterLevel; }

	void setSpeedModifier(float _modifier, double _duration);

	void update(double _dt);
	void reverseFlow();
	void pauseFlow(double _duration);
};


