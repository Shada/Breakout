#pragma once
#include "Brick.h"
#include "Ball.h"
#include "Pad.h"
#include "Water.h"
#include "Skybox.h"
#include "Resource.h"
#include "Text.h"
#include "Effect.h"
#include "UIElement.h"
namespace Logic
{
	class ObjectCore
	{
	public:
		//----------------------------------
		//gameplay
		//----------------------------------
		enum MapType
		{
			eTest = 0,
			eWind = 1,
			eWater = 2,
			eFire = 3,
			eEarth = 4
		};
		
		Pad						*pad;
		std::vector<Ball*>		ball;
		std::vector<Brick*>		bricks;
		std::vector<Effect*>	effects;
		
		std::vector<BBUI>		uiBillboards;
		std::vector<BBFont>		fontBillboards;

		Font					*testFont;
		Text					*testText;
		
		Water					*water;
		Skybox					*skybox;

		UIElement				*SideBar;
		UIElement				*statusBar;

		//----------------------------------
		//menu
		//----------------------------------
		std::vector<Text>			optionList;
		std::vector<UIElement>		gui;

		UIElement					*selector;
		UIElement					*logo;
		UIElement					*loli;
		UIElement					*circle;

		ObjectCore();
		~ObjectCore();

		void setMapType(int type);
		MapType getMapType() { return mapType; }

		MapType mapType;
	};
}

