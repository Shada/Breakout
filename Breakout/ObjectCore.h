#pragma once
#include "Brick.h"
#include "Ball.h"
#include "Pad.h"
#include "Water.h"
#include "Skybox.h"
#include "Resource.h"
#include "Text.h"

namespace Logic
{
	class ObjectCore
	{
	public:
		enum MapType
		{
			eTest = 0,
			eWind = 1,
			eWater = 2,
			eFire = 3,
			eEarth = 4
		};
		
		Pad						*pad;
		std::vector<Brick*>		bricks;
		std::vector<Ball*>		ball;
		
		std::vector<BBUI>		uiBillboards;
		std::vector<BBFont>		fontBillboards;

		Font					*testFont;
		Text					*testText;
		
		Water					*water;
		Skybox					*skybox;

		ObjectCore();
		~ObjectCore();

		void setMapType(int type);
		MapType getMapType() { return mapType; }

		MapType mapType;
	};
}

