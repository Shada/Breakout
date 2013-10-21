#include "Brick.h"
#include "Physics.h"

namespace Logic
{
	Brick::Brick(Vec3 _pos, bool _cylinder)
	{
		position = _pos;
		scale		= Vec3(1,1,1);
		rotation	= Vec3(0,0,0);

		alive = true;
		health = 1;

		width = 5;
		height = 5;

		if(_cylinder)
			transformToCyl();
		else
			updateWorld();
		
	}

	Brick::Brick(Vec3 _pos, bool _cylinder, double _width, double _height)
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

		if(_cylinder)
			transformToCyl();
		else
			updateWorld();
	}

	Brick::~Brick()
	{

	}

	void Brick::update(double _dt)
	{

	}

	bool Brick::isDestroyed()
	{
		if (health <= 0)
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

	void Brick::setType(int _Type)
	{
		health = 1;

		if(_Type == 48)
			health = 2;
		else if(_Type == 60)
			health = 3;

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