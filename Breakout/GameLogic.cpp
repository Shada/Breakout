#include "GameLogic.h"
#include <iostream>
namespace Logic
{
	GameLogic::GameLogic(Inputhandler *handler)
	{
		pad = new Pad();
		ball = new Ball();
		camera = new Camera();

		inputHandler = handler;
		std::vector<KeyBind> keys;

		keys.push_back(KeyBind(DIK_UPARROW, &pad->rotateLeft));
		keys.push_back(KeyBind(DIK_DOWNARROW, &pad->rotateRight));
		keys.push_back(KeyBind(DIK_LEFTARROW, &pad->moveRight));
		keys.push_back(KeyBind(DIK_RIGHTARROW, &pad->moveRight));

		inputHandler->setPad(pad, keys);
		//inputHandler->setCamera(camera, keys);

		bricks.push_back(new Brick(Vec3(100, 150, 0)));
		bricks.push_back(new Brick(Vec3(170, 150, 0)));
		bricks.push_back(new Brick(Vec3(240, 150, 0)));
		bricks.push_back(new Brick(Vec3(310, 150, 0)));
		bricks.push_back(new Brick(Vec3(380, 150, 0)));
		bricks.push_back(new Brick(Vec3(450, 150, 0)));
		bricks.push_back(new Brick(Vec3(520, 150, 0)));
		bricks.push_back(new Brick(Vec3(100, 200, 0)));
		bricks.push_back(new Brick(Vec3(170, 200, 0)));
		bricks.push_back(new Brick(Vec3(240, 200, 0)));
		bricks.push_back(new Brick(Vec3(310, 200, 0)));
		bricks.push_back(new Brick(Vec3(380, 200, 0)));
		bricks.push_back(new Brick(Vec3(450, 200, 0)));
		bricks.push_back(new Brick(Vec3(520, 200, 0)));

		state = GAME_PLAY;
	}

	void GameLogic::update(double _dt)
	{
		switch(state)
		{
		case GAME_PLAY:
		{
			inputHandler->updateGame();
			pad->update(_dt);
			ball->update(_dt);
			camera->update();

			int collidingObject = Logic::Check2DCollissions(ball, bricks);
			if(collidingObject != -1)
			{
				SAFE_DELETE(bricks.at(collidingObject));
				bricks.erase(bricks.begin() + collidingObject, bricks.begin() + collidingObject + 1);
				std::cout << "Collided with a brick yo! Only " << bricks.size() << " left!!!!" << std::endl;
			}

			//std::cout << pad->getPosition().x << std::endl;

			break;
		}
		case GAME_MENU:
			inputHandler->updateMenu();
			break;
		}
	}

	void GameLogic::draw()
	{
		//pad->draw();
	}

	GameLogic::~GameLogic()
	{
		SAFE_DELETE(camera);
		SAFE_DELETE(pad);
		SAFE_DELETE(inputHandler);
		SAFE_DELETE(ball);
		for(unsigned int i = 0; i < bricks.size(); i++)
			SAFE_DELETE(bricks.at(i));
	}
}