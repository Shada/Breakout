#include "Ball.h"
#include "Physics.h"

namespace Logic
{

	Ball::Ball() : Object3D()
	{
		scale		= Vec3(1,1,1);
		position	= Vec3(0,0,0);
		rotation	= Vec3(0,0,0);
		textureID = 0;
		direction = Vec3(1, 1, 0);
		radius = 3.09543991f;
		direction.normalize();
		speed = 100;
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
		//Check for buffs/debuffs here, and apply them
		lastFrame = position;
		position += direction * speed * (float)_dt;

		updateWorld();
	}

	void Ball::updateCylinder(double _dt)
	{
		//Check for buffs/debuffs here, and apply them
		lastFrame = position;
		position += direction * speed * (float)_dt;

		if(position.x > 300.0f || position.x < 0.0f)
		{
			position.x > 300.0f ? position.x -= 300.f : position.x += 300.f;
		}

		transformToCyl();
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
        if(_x != NULL) direction.x = _x;
		if(_y != NULL) direction.y = _y;
		if(_z != NULL) direction.z = _z;
    }
#endif // _WIN32
}
