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
		//Logik för att hantera växlandet mellan menyer och gameplay
		//Logik för att kalla på interfaces
		
		Inputhandler *inputHandler;
		
	public:
		GameLogic(Inputhandler *handler);
		~GameLogic();

		void update(double dt);
		void draw();
	};

}
#endif // ! _GAMELOGIC_H_