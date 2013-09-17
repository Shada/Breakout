#include "Brick.h"
namespace Logic
{
	Brick::Brick(Vec3 _pos)
	{
		position = _pos;
		alive = true;
	}

	Brick::~Brick()
	{

	}

	void Brick::update(double _dt)
	{
		
	}

	void Brick::damage()
	{
		health--;
		if(health < 0) this->destroy();
	}

	void Brick::destroy()
	{
		//remove hitbox;
		alive = false;
	}

	
}