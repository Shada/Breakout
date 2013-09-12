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
		/*
		Do collision check here
		*/
		position += direction*speed;
	}
}

