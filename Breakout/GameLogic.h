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

		enum GameState
		{
			GAME_MENU = 0,
			GAME_PLAY = 1
		};
		

		//Logik f�r att hantera v�xlandet mellan menyer och gameplay
		//Logik f�r att kalla p� interfaces
		Pad *pad;
		std::vector<Object3D*> bricks;
		Ball *ball;
		Camera *camera;
		Inputhandler *inputHandler;

		GameState state;
		
	public:
		GameLogic(Inputhandler *handler);
		~GameLogic();

		void update(double dt);
		void draw();
	};

}
#endif // ! _GAMELOGIC_H_