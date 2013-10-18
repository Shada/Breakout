#pragma once
#include "Brick.h"
#include "Ball.h"
#include "Pad.h"
#include "Resource.h"
#include "Text.h"
#include "UIElement.h"
namespace Logic
{
	class ObjectCore
	{
	public:
		ObjectCore();
		Pad						*pad;
		std::vector<Object3D*>	bricks;
		Ball					*ball;

		std::vector<BBUI>		uiBillboards;
		std::vector<BBFont>		fontBillboards;

		Font					*testFont;
		Text					*testText;

		UIElement				*SideBar;

		~ObjectCore();
	};
}

