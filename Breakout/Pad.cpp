#include "Pad.h"

namespace Logic
{
	Vec3 Pad::pos = Vec3(0, 0, 0);
	Pad::Pad()
	{

	}

	Pad::Pad()
	{
		movementSpeed = 1.0f;
		angle2D = 0.0f;
		angle3D = 0.0f;
	}

	Pad::~Pad()
	{

	}

	void Pad::Update(double _dt)
	{
		//Calculate on buffs and debuffs
		position = pos;
	}

	void Pad::Move2D(double _dt, float _x)
	{
		position.x += _x * movementSpeed * _dt;
	}

	void Pad::Move3D(double _dt, float _x)
	{
		//Do 3D movement (in a circle) here.
		angle3D += _x * movementSpeed * _dt; //Assume degrees

		if(angle3D < 0.0f) angle3D += 360.0f;
		else if(angle3D >= 360.0f) angle3D -= 360.0f;
		
		position.x = cosf(angle3D); //Some sort of radius on the circle should be added.
		position.y = sinf(angle3D);
	}

	void Pad::Move(int pixels)
	{
		pos.x += pixels;
		//position.x += pixels;
	}


}
