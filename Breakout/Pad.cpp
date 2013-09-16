#include "Pad.h"

namespace Logic
{
	Vec3 Pad::pos = Vec3(0, 0, 0);
	Pad::Pad()
	{

	}

	void Pad::update(double dt)
	{
		position = pos;
	}

	void Pad::move(int pixels)
	{
		pos.x += pixels;
		//position.x += pixels;
	}

	Pad::~Pad()
	{

	}
}