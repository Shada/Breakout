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
			eTest,
			eWind,
			eWater,
			eFire,
			eEarth
		};
		MapType mapType;
		ObjectCore();
		Pad						*pad;
		std::vector<Object3D*>	bricks;
		Ball					*ball;
		Water					*water;
		Skybox					*skybox;

		std::vector<BBUI>		uiBillboards;
		std::vector<BBFont>		fontBillboards;

		Font					*testFont;
		Text					*testText;

		~ObjectCore();
	};
}

