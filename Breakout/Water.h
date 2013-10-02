#pragma once
class Water
{
private:
	float waterLevel;
	float maxSpeed;
	float speed;
	float acceleration;

	// used for modifier
	float speedModifier;
	double speedDuration;

	// used for pausing the water rising
	bool paused;
	double pausedDuration;

public:
	/* parameter sets the initial water level */
	Water(float _waterLevel);
	~Water();

	float getWaterLevel() { return waterLevel; }

	void setSpeedModifier(float _modifier, double _duration);

	void update(double _dt);
	void reverseFlow();
	void pauseFlow(double _duration);
};


