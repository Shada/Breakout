#ifndef _GAMELOGIC_H_
#define _GAMELOGIC_H_

#include "Map.h"
#include "Pad.h"
#include "Inputhandler.h"
#include "Camera.h"
#include "Ball.h"
#include "Physics.h"
#include "Brick.h"
#include "Resource.h"
#include <functional>

namespace Logic
{
	class GameLogic
	{
	private:
		//TODO:
		//GameplayClass
		//MenuClass
		//List of Maps

		//Logik f�r att hantera v�xlandet mellan menyer och gameplay
		//Logik f�r att kalla p� interfaces
		Pad *pad;
		std::vector<Object3D*> bricks;
		Ball *ball;
		Camera *camera;
		Inputhandler *inputHandler;
		
	public:
		GameLogic(Inputhandler *handler);
		~GameLogic();

		void update(double dt);
		void draw();
	};

}
#endif