#pragma once
#include "Brick.h"
#include "Ball.h"
#include "Pad.h"
#include "Resource.h"
#include "Font.h"
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

		Font					*testFont;

		~ObjectCore();
	};
}

