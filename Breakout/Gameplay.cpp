#include "Gameplay.h"
#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#include <iostream>
#endif

namespace Logic
{
	Gameplay::Gameplay(Inputhandler *&_handler)
	{
		mapLoading = new Map();
		//tolka Map och skapa object enligt den
		pad = new Pad();
		ball = new Ball();
		camera = new Camera();

		std::vector<KeyBind> keys;
#ifdef _WIN32
		keys.push_back(KeyBind(DIK_UPARROW, &pad->rotateLeft));
		keys.push_back(KeyBind(DIK_DOWNARROW, &pad->rotateRight));
		keys.push_back(KeyBind(DIK_LEFTARROW, &pad->moveRight));
		keys.push_back(KeyBind(DIK_RIGHTARROW, &pad->moveRight));
#endif
		_handler->setPad(pad, keys);
		//inputHandler = handler;

		//inputHandler->setCamera(camera, keys);

		mapLoading->loadMap(0,&bricks,ball,pad);

	}



	void Gameplay::update(double _dt)
	{
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
	}

	void Gameplay::draw()
	{
		//Resources::LoadHandler::getInstance()->getModel(ball->getModelID())->draw();
		ball->draw();

		for(unsigned int i = 0; i < bricks.size(); i++)
		{
			bricks.at(i)->draw();
		}
	}

	Gameplay::~Gameplay()
	{
		SAFE_DELETE(camera);
		SAFE_DELETE(pad);
		SAFE_DELETE(ball);
		for(unsigned int i = 0; i < bricks.size(); i++)
			SAFE_DELETE(bricks.at(i));
	}
}
