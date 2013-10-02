#include "GameLogic.h"
#include "LoadHandler.h"
#include <iostream>
#include "Resource.h"
namespace Logic
{
	GameLogic::GameLogic(Inputhandler *handler)
	{
		soundSystem = new SoundSystem();
		soundSystem->Initialize();
		soundSystem->Play(1);

		inputHandler = handler;
		gameplay = new Gameplay(inputHandler, soundSystem);


		//gameState = GameState::GAME_PLAY;

		Resources::LoadHandler::getInstance();
	}

	void GameLogic::update(double _dt)
	{
		soundSystem->Update(_dt);

		inputHandler->updateGame();
		gameplay->update(_dt);
		/*switch(gameState)
		{
		case GAME_PLAY:
		{
			
			break;
		}
		case GAME_PLAYING:
			inputHandler->updateGame();
			gameplay->update(_dt);
			break;
		case GAME_MENU:
			inputHandler->updateMenu();
			break;
		}*/
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
		SAFE_DELETE(soundSystem);
	}
}
