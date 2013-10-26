#include "GameLogic.h"
#include "LoadHandler.h"
#include <iostream>
#include "Resource.h"
namespace Logic
{
	GameLogic::GameLogic(Inputhandler *handler)
	{
		///soundSystem = new SoundSystem();
		///soundSystem->Initialize();
		///soundSystem->Play(1);

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

		gameplay		= new Gameplay(inputHandler/**, soundSystem*/, objectCore);
		inputHandler	= handler;
		inputHandler->setMenu(menu, keys);

		Resources::LoadHandler::getInstance();

		Global::getInstance()->gameState = GameState::GAME_MENU;
	}

	void GameLogic::update(float _dt)
	{
///		soundSystem->Update(_dt);

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
			/*if(GetAsyncKeyState(VK_DOWN) != 0)
			{
				menu->moveDown();
			}
			else if(GetAsyncKeyState(VK_UP) != 0)
			{
				menu->moveUp();
			}
			else if(GetAsyncKeyState(VK_SPACE) != 0 || GetAsyncKeyState(VK_RETURN) != 0)
			{
				menu->confirm();*/
				//if(menu->getSelectedOptionInt() == 0)
				//	menu->close();
			//}
			menu->update(_dt);
			if(!menu->isOpen())
				Global::getInstance()->gameState = GAME_PLAY;
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
		///SAFE_DELETE(soundSystem);
		SAFE_DELETE(menu);
	}
}
