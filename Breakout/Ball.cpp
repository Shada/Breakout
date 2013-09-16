#include "Ball.h"

namespace Logic
{
	
	Ball::Ball() : Object3D()
	{
	
	}
	
	Ball::~Ball()
	{
	
	}
	
	void Ball::Update(double _dt)
	{
		//Check for buffs/debuffs here, and apply them
		position += direction * speed * _dt;
		nextFrame = position + (direction * speed * _dt);
	}

	void Ball::setDirection(float _x = NULL, float _y = NULL, float _z = NULL)
	{
		if(_x != NULL) direction.x = _x;
		if(_y != NULL) direction.y = _y;
		if(_z != NULL) direction.z = _z;
	}
}

