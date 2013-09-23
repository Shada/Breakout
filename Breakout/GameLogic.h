#ifndef _GAMELOGIC_H_
#define _GAMELOGIC_H_

#include "Resource.h"
#include "Gameplay.h"
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
		
		Gameplay *gameplay;
		//Logik f�r att hantera v�xlandet mellan menyer och gameplay
		//Logik f�r att kalla p� interfaces
		
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