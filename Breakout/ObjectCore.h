#pragma once
#include "Brick.h"
#include "Ball.h"
#include "Pad.h"
namespace Logic
{
	class ObjectCore
	{
	public:
		enum MapType
		{
			eTest,
			eWind,
			eWater,
			eFire,
			eEarth
		};
		MapType mapType;
		ObjectCore();
		Pad						*pad;
		std::vector<Brick*>		 bricks;
		Ball					*ball;

		~ObjectCore();
	};
}

