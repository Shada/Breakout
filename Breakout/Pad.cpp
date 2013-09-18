#include "Pad.h"
#include "Resource.h"

namespace Logic
{
	Vec3 Pad::posKey = Vec3(0, 0, 0);
	Vec3 Pad::posMouse = Vec3(0, 0, 0);
	Vec3 Pad::rot = Vec3(0, 0, 0);
	/*Pad::Pad()
	{

	}*/

	Pad::Pad()
	{
		movementSpeed = 1.0f;
		angle2D = 0.0f;
		angle3D = 0.0f;
	}

	Pad::~Pad()
	{

	}

	void Pad::update(double _dt)
	{
		//Calculate on buffs and debuffs

		if(posMouse.x != position.x)
			position.x = posMouse.x;
		else if(posKey.x != position.x)
		{
			position.x = posKey.x;
		}
		//position.x = pos.x;
		rotation.x = rot.x;

		posMouse.x = posKey.x = position.x;
	}

	void Pad::move2D(double _dt, float _x)
	{
		position.x += _x * movementSpeed * _dt;
	}

	void Pad::move3D(double _dt, float _x)
	{
		//Do 3D movement (in a circle) here.
		angle3D += _x * movementSpeed * _dt; //Assume degrees

		if(angle3D < 0.0f) angle3D += 360.0f;
		else if(angle3D >= 360.0f) angle3D -= 360.0f;
		
		position.x = cosf(angle3D); //Some sort of radius on the circle should be added.
		position.y = sinf(angle3D);
	}

	void Pad::move(int _pixels)
	{
		posMouse.x += _pixels;
	}

	void Pad::moveByKeys(int _direction)
	{
		_direction > 0 ? posKey.x += 5 : posKey.x -= 5;
	}

	void Pad::rotate(int _direction)
	{
		// Theoretically, the mouse wheel cannot be rotated more than 1 tick during 1 frame
		// This means that the input will always be 120 from delta z, which in our program will mean 12 degrees
		rot.x += 12 * PI / 180 * _direction;
		if(rot.x > 2 * PI)
			rot.x -= 2 * PI;
	}
}
