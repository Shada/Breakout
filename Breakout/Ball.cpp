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
		textureID = 0;
		direction = Vec3(1, 1, 0);
		radius = 3.09543991f;
		direction.normalize();
		speed = 200;
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

    void Ball::setDirection(float _x, float _y, float _z)
    {
        if(_x != NULL) direction.x = _x;
        if(_y != NULL) direction.y = _y;
        if(_z != NULL) direction.z = _z;
    }
}
