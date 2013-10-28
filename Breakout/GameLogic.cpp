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
		//soundSystem->Play(1);

		inputHandler	= handler;
		objectCore		= new ObjectCore();

		menu			= new Menu(objectCore);
		menu->addOption("New Game");
		menu->addOption("Highscore");
		menu->addOption("Options");
		menu->addOption("Credits");
		menu->addOption("Exit");
		menu->open();

		std::vector<KeyBind2> keys;
		keys.push_back(KeyBind2(KC_UP, &menu->moveUp));
		keys.push_back(KeyBind2(KC_DOWN, &menu->moveDown));
		keys.push_back(KeyBind2(KC_RETURN, &menu->confirm));

		gameplay		= new Gameplay(inputHandler, soundSystem, objectCore);
		inputHandler	= handler;
		inputHandler->setMenu(menu, keys);
		
		Resources::LoadHandler::getInstance();

		Global::getInstance()->gameState = GameState::GAME_MENU;
	}

	void GameLogic::update(float _dt)
	{
		soundSystem->Update(_dt);

		switch(Global::getInstance()->gameState)
		{
		case GAME_PLAY:
		{
			inputHandler->updateGame();
			gameplay->update(_dt);
			break;
		}
		case GAME_MENU:
			inputHandler->updateMenu();
			menu->update(_dt);
			break;
		}
	}

	GameLogic::~GameLogic()
	{
		SAFE_DELETE(inputHandler);
		SAFE_DELETE(gameplay);
		Resources::LoadHandler *lh = Resources::LoadHandler::getInstance();
		SAFE_DELETE(lh);
		SAFE_DELETE(soundSystem);
		SAFE_DELETE(menu);
	}
}
