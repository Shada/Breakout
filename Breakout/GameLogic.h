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
		
		Gameplay *gameplay;
		//Logik f�r att hantera v�xlandet mellan menyer och gameplay
		//Logik f�r att kalla p� interfaces
		
		Inputhandler *inputHandler;
		
	public:
		GameLogic(Inputhandler *handler);
		~GameLogic();

		void update(double dt);
		void draw();
	};

}
#endif // ! _GAMELOGIC_H_