#include "GameLogic.h"
#include "LoadHandler.h"
#include <iostream>
#include "Resource.h"
namespace Logic
{
	GameLogic::GameLogic(Inputhandler *handler)
	{
		inputHandler = handler;

		menu		= new Menu();
		gameplay	= new Gameplay(inputHandler);
		gameState	= GAME_PLAY;

		Resources::LoadHandler::getInstance();
	}

	void GameLogic::update(double _dt)
	{
		switch(gameState)
		{
		case GAME_PLAY:
		{
			
			inputHandler->updateGame();
			//std::cout << pad->getPosition().x << std::endl;
			gameplay->update(_dt);
			break;
		}
		case GAME_MENU:
			inputHandler->updateMenu();
			break;
		}
	}

	GameLogic::~GameLogic()
	{
		/*SAFE_DELETE(camera);
		SAFE_DELETE(pad);*/
		SAFE_DELETE(inputHandler);
		/*SAFE_DELETE(ball);
		for(unsigned int i = 0; i < bricks.size(); i++)
			SAFE_DELETE(bricks.at(i));*/
		SAFE_DELETE(gameplay);
		Resources::LoadHandler *lh = Resources::LoadHandler::getInstance();
		SAFE_DELETE(lh);
	}
}