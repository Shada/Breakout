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
		std::vector<int> keys, directions;
		std::vector<std::function<void(int)>> funcs;
		std::vector<std::function<void(int, int)>> funcss;

		funcs.push_back(&pad->move);
		funcs.push_back(&pad->move);
		funcs.push_back(&pad->rotate);
		funcs.push_back(&pad->rotate);


		//Only works for DirectX atm, you need to add a direction as well.
		//Should rather be functions for right- and left movement, instead of sending directions.
		//Make a struct with {key, functionptr} pairs?
		keys.push_back(DIK_RIGHTARROW);
		directions.push_back(1);
		keys.push_back(DIK_LEFTARROW);
		directions.push_back(-1);


		//std::function<void(int)> padMove = &pad->move;

		inputHandler->setPad(pad, keys, funcs, directions);
		inputHandler->setCamera(camera, keys, funcss);

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