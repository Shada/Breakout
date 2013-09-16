#include "Brick.h"
namespace Logic
{
	Brick::Brick()
	{
		alive = true;
	}

	Brick::~Brick()
	{

	}
	void Brick::Update(double _dt)
	{
		
	}

	void Brick::Damage()
	{
		health--;
		if(health < 0) this->Destroy();
	}

	void Brick::Destroy()
	{
		//remove hitbox;
		alive = false;
	}

	
}