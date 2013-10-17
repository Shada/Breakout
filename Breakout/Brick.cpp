#include "Brick.h"
#include "Physics.h"

namespace Logic
{
	Brick::Brick(Vec3 _pos)
	{
		position = _pos;
		scale		= Vec3(1,1,1);
		rotation	= Vec3(0,0,0);

		alive = true;
		health = 1;

		width = 5;
		height = 5;

		updateWorld();
		//transformToCyl();
	}

	Brick::Brick(Vec3 _pos, double _width, double _height)
	{
		position = _pos;
		scale		= Vec3(1,1,1);
		rotation	= Vec3(0,0,0);

		alive = true;
		health = 1;

		width = _width;
		height = _height;

		scale.x = (float)width / 5;
		scale.y =  (float)height / 5;

		transformToCyl();
	}

	Brick::~Brick()
	{

	}

	void Brick::update(double _dt)
	{

	}

	bool Brick::isDestroyed()
	{
		if (health == 0)
			return true;
		return false;
	}

	void Brick::damage()
	{
		health--;
		if (health == 1)
			setTextureID(1);
		else if (health == 2)
			setTextureID(6);
		//if(health < 0) this->destroy();
	}

	void Brick::destroy()
	{
		//remove hitbox;
		alive = false;
	}

	void Brick::setHeight( double _h)
	{
		height = _h; 
		scale.y =  (float)height / 5;
	}

	void Brick::setWidth( double _w)
	{
		 width = _w; 
		 scale.x = (float)width / 5;
	}
}