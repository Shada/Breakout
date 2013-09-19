#include "GameLogic.h"
#include "LoadHandler.h"
#include <iostream>
namespace Logic
{
	GameLogic::GameLogic(Inputhandler *handler)
	{
		inputHandler = handler;
		gameplay = new Gameplay(inputHandler);

		state = GAME_PLAY;

		Resources::LoadHandler::getInstance();
	}

	void GameLogic::update(double _dt)
	{
		switch(state)
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

	void GameLogic::draw()
	{
		switch(state)
		{
		case GAME_PLAY:
			gameplay->draw();
			break;
		case GAME_MENU:
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