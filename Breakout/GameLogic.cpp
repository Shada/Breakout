#include "GameLogic.h"

namespace Logic
{
	GameLogic::GameLogic(Inputhandler *handler)
	{
		pad = new Pad();
		camera = new Camera();

		inputHandler = handler;
		std::vector<int> keys;
		std::vector<std::function<void()>> funcs;
		std::vector<std::function<void(int, int)>> funcss;

		std::function<void(int)> padMove = &pad->move;

		inputHandler->setPad(pad, keys, funcs, padMove);
		inputHandler->setCamera(camera, keys, funcss);
	}

	void GameLogic::update(double dt)
	{
		inputHandler->update();
		pad->update(dt);
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
	}
}