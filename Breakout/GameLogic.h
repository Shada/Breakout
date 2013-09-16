#ifndef _GAMELOGIC_H_
#define _GAMELOGIC_H_

#include "Map.h"
#include "Pad.h"
#include "Inputhandler.h"
#include "Camera.h"
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

		//Logik för att hantera växlandet mellan menyer och gameplay
		//Logik för att kalla på interfaces
		Pad *pad;
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