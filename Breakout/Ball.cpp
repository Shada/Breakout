#include "Ball.h"
namespace Logic
{
	
	Ball::Ball() : Object3D()
	{
		position = Vec3(100, 90, 0);
		direction = Vec3(1, 1, 0);
		radius = 25;
		direction.normalize();
		speed = 100;
	}
	
	Ball::~Ball()
	{
	
	}
	
	void Ball::update(double _dt)
	{
		//Check for buffs/debuffs here, and apply them
		position += direction * speed * (float)_dt;
		nextFrame = position + (direction * speed * (float)_dt);
	}
	void Ball::draw()
	{

	}
	void Ball::setDirection(float _x, float _y, float _z)
	{
		if(_x != NULL) direction.x = _x;
		if(_y != NULL) direction.y = _y;
		if(_z != NULL) direction.z = _z;
	}
}