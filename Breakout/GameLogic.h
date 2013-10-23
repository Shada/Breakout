#ifndef _GAMELOGIC_H_
#define _GAMELOGIC_H_

#include "Resource.h"
#include "Gameplay.h"
#include "SoundSystem.hpp"
#include <functional>
#include "Menu.h"


namespace Logic
{
	class GameLogic
	{
	private:
		SoundSystem		*soundSystem;
		Gameplay		*gameplay;
		//Logik f�r att hantera v�xlandet mellan menyer och gameplay
		//Logik f�r att kalla p� interfaces
		
		Inputhandler	*inputHandler;
		Menu			*menu;

		ObjectCore		*objectCore;

	public:
		GameLogic(Inputhandler *handler);
		~GameLogic();

		void update(float dt);
		void draw();
		void setFpsCounter(int _fps) { gameplay->setFps(_fps); }
	};

}
#endif // ! _GAMELOGIC_H_