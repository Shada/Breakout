#include "Ball.h"
#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#endif // _WIN32

namespace Logic
{

	Ball::Ball() : Object3D()
	{
		scale		= Vec3(1,1,1);
		position	= Vec3(0,0,0);
		rotation	= Vec3(0,0,0);

		direction = Vec3(1, 1, 0);
		effectDirection = Vec3(1, 1, 0);
		activeEffect = 0;
		radius = 3.09544444f;
		direction.normalize();
		speed = 100;
		srand (time(NULL));

#ifdef _WIN32
		shaderTechniqueID = GraphicsDX11::getInstance()->getTechIDByName("techSimple");
#endif
	}

	Ball::~Ball()
	{
	}

	void Ball::update(double _dt)
	{
		//effect calculations
		if (activeEffect == 1)//wind effect
		{
			effectTimer -= _dt;
			if (effectTimer > 0)
			{
				direction = (direction + effectDirection *_dt);
				direction.normalize();
				effectSpeed += (effectAcceleration * _dt);
			}
			else
				effectSpeed -= (effectAcceleration * _dt);
			if(effectSpeed > 0 && effectTimer < 0)
			{
				effectDirection = Vec3(1, 1, 0);
				effectSpeed = 0;
				activeEffect = 0;
			}
			position += direction * speed * (float)_dt + effectDirection * effectSpeed * (float)_dt;
			nextFrame = position + direction * speed * (float)_dt + effectDirection * effectSpeed * (float)_dt;
}
		else // no effects
		{		
			position += direction * speed * (float)_dt;
			nextFrame = position + direction * speed * (float)_dt;
		}

		updateWorld();

		 
	}

	void Ball::startWind()
	{
		if (activeEffect == 0)
		{
			effectDirection = Vec3((rand()%10)-5, (rand()%10)-5, 0);
			effectDirection.normalize();
			effectSpeed = 0;
			effectTimer = 1.5;
			activeEffect = 1;
			effectAcceleration = 30;
		}
	}

#ifdef _WIN32
	void Ball::setDirection(float _x, float _y, float _z)
	{
		if(_x != NULL) direction.x = _x;
		if(_y != NULL) direction.y = _y;
		if(_z != NULL) direction.z = _z;
	}
#else
    void Ball::setDirection(float _x, float _y, float _z)
    {
        direction.x = _x;
        direction.y = _y;
        direction.z = _z;
    }
#endif // _WIN32

}
