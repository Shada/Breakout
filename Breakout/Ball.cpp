#include "Ball.h"
#include "Physics.h"

namespace Logic
{

	Ball::Ball() : Object3D()
	{
		scale		= Vec3(1,1,1);
		position	= Vec3(0,0,0);
		rotation	= Vec3(0,0,0);
		direction	= Vec3(1, 1, 0);
		effectDirection = Vec3(1, 1, 0);

		textureID = 0;
		radius = 3.09543991f;
		activeEffect = 0;
		direction.normalize();
		speed = 100;
		srand((unsigned)time(NULL));
	}

	Ball::~Ball()
	{
	}

	void Ball::setPosition(Vec3 _pos)
	{
		position = _pos;
	}

	void Ball::update(double _dt)
	{
		float dt = (float)_dt;
		//Check for buffs/debuffs here, and apply them

		//effect calculations
		if (activeEffect == 1)//wind effect
		{
			effectTimer -= dt;
			if (effectTimer > 0)
			{
				direction = (direction + effectDirection * dt);
				direction.normalize();
				effectSpeed += (effectAcceleration * dt);
			}
			else
				effectSpeed -= (effectAcceleration * dt * 2);
			if(effectSpeed > 0 && effectTimer < 0)
			{
				effectDirection = Vec3(1, 1, 0);
				effectSpeed = 0;
				activeEffect = 0;
			}
			lastFrame = position;
			position += direction * speed * dt + effectDirection * effectSpeed * dt;
		}
		else // no effects
		{		
			lastFrame = position;
			position += direction * speed * dt;
		}

		updateWorld();
	}

	void Ball::updateCylinder(double _dt)
	{
		//Check for buffs/debuffs here, and apply them
		lastFrame = position;
		position += direction * speed * (float)_dt;

		if(position.x > Logic::borderMaxX || position.x < 0.0f)
		{
			position.x > Logic::borderMaxX ? position.x -= Logic::borderMaxX : position.x += Logic::borderMaxX;
		}

		transformToCyl();
	}

	void Ball::setDirection(float _x, float _y, float _z)
	{
		if(_x != NULL) direction.x = _x;
		if(_y != NULL) direction.y = _y;
		if(_z != NULL) direction.z = _z;
		direction.normalize();
	}

	void Ball::startWind()
	{
		if (activeEffect == 0)
		{
			effectDirection = Vec3((rand()%10)-5, (rand()%10)-5, 0);
			effectDirection.normalize();
			effectSpeed = 0;
			effectTimer = 1.6;
			activeEffect = 1;
			effectAcceleration = 30;
		}
	}

	/*void Ball::setWindDirection(float _x, float _y, float _z)
	{
		if (activeEffect == 0)
		{
			effectDirection = Vec3((float)(rand() % 10) - 5, (float)(rand() % 10) - 5, 0);
			effectDirection.normalize();
			effectSpeed = 0;
			effectTimer = 1.6f;
			activeEffect = 1;
			effectAcceleration = 30;
		}
	}*/
}
