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
		std::vector<int> keys;
		std::vector<std::function<void()>> funcs;
		std::vector<std::function<void(int, int)>> funcss;

		std::function<void(int)> padMove = &pad->move;

		inputHandler->setPad(pad, keys, funcs, padMove);
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
	}

	void GameLogic::update(double dt)
	{
		inputHandler->update();
		pad->update(dt);
		ball->update(dt);
		int temp = Logic::Check2DCollissions(ball, bricks);
		if(temp != -1)
		{
			SAFE_DELETE(bricks.at(temp));
			bricks.erase(bricks.begin() + temp, bricks.begin() + temp + 1);
			std::cout << "Collided with a brick yo! Only " << bricks.size() << " left!!!!" << std::endl;
		}
		if(bricks.size() == 0)
			int a = 0;

		camera->update();
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