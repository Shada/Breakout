#include "Gameplay.h"
#include "GraphicsDX11.h"

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
		keys.push_back(KeyBind(DIK_UPARROW, &pad->rotateLeft));
		keys.push_back(KeyBind(DIK_DOWNARROW, &pad->rotateRight));
		keys.push_back(KeyBind(DIK_LEFTARROW, &pad->moveLeft));
		keys.push_back(KeyBind(DIK_RIGHTARROW, &pad->moveRight));

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

		//if(GetAsyncKeyState(VK_END) != 0)
			//ball->setModelID(1);

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
		pad->draw();

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