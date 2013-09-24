#pragma once
#include "Brick.h"
#include "Ball.h"
#include "Pad.h"
namespace Logic
{
	class ObjectCore
	{
	public:
		ObjectCore();

		Pad *pad;
		std::vector<Object3D*> bricks;
		Ball *ball;

		~ObjectCore();
	};
}

