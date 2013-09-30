#include "Ball.h"
#ifdef BAJSAPA
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#endif // BAJSAPA

namespace Logic
{

	Ball::Ball() : Object3D()
	{
		scale		= Vec3(1,1,1);
		position	= Vec3(0,0,0);
		rotation	= Vec3(0,0,0);

		direction = Vec3(1, 1, 0);
		radius = 3.09544444f;
		direction.normalize();
		speed = 100;

#ifdef BAJSAPA
		shaderTechniqueID = GraphicsDX11::getInstance()->getTechIDByName("techSimple");
#endif
	}

	Ball::~Ball()
	{
	}

	void Ball::update(double _dt)
	{
		//Check for buffs/debuffs here, and apply them
		position += direction * speed * (float)_dt;
		nextFrame = position + (direction * speed * (float)_dt);

		updateWorld();
	}

#ifndef BAJSAPA
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
#endif // BAJSAPA

}
