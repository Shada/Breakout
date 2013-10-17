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
		//tolka Map och sk_WIN32 object enligt den
		objectCore = new ObjectCore();
		play = ballPadCollided = false;

		#ifdef _WIN32
		GraphicsDX11::getInstance()->setObjectCore(objectCore);
		#else
		GraphicsOGL4::getInstance()->setObjectCore(objectCore);
		#endif

		objectCore->ball->setModelID(0);
		camera = new Camera();
	/*	Logic::sph2Cart(Vec3(0,1.570796,39));
		Logic::cart2Sph(Vec3(39,0,0));*/

		//camera->setPosition(Logic::fitToScreen(Vec3(-384,256,0), Vec3(384,256,0), Vec3(-384,0,0), Vec3(384,0,0)));
		camera->setPosition(Logic::fitToScreen(Vec3(0,200,0), Vec3(300,200,0), Vec3(0,0,0), Vec3(300,0,0)));
		Logic::calculateCameraBorders(camera->getPosition(), -camera->getPosition().z, (float)(4.f / 3));

		std::vector<KeyBind> keys;
		keys.push_back(KeyBind(KC_UP, &objectCore->pad->rotateLeft));
		keys.push_back(KeyBind(KC_DOWN, &objectCore->pad->rotateRight));
		//keys.push_back(KeyBind(KC_LEFT, &objectCore->pad->moveLeft));
		//keys.push_back(KeyBind(KC_RIGHT, &objectCore->pad->moveRight));
		keys.push_back(KeyBind(KC_SPACE, &objectCore->pad->ejectBall));
		keys.push_back(KeyBind(KC_RIGHT, &objectCore->pad->moveLeft));
		keys.push_back(KeyBind(KC_LEFT, &objectCore->pad->moveRight));
		_handler->setPad(objectCore->pad, keys);
		//inputHandler = handler;

		//inputHandler->setCamera(camera, keys);

		currentMapIndex = 0;
		mapLoading->loadMap(currentMapIndex,&objectCore->bricks,objectCore->ball,objectCore->pad);

	}

	void Gameplay::update(double _dt)
	{
		//objectCore->pad->update(_dt);
		objectCore->pad->updateCylinder(_dt);

		if(play)
		{
			//objectCore->ball->update(_dt);
			objectCore->ball->updateCylinder(_dt);
			if(!ballPadCollided)
				ballPadCollided = Logic::ballCollision(objectCore->ball, objectCore->pad, objectCore->pad->getRotation().z);
			else
				ballPadCollided = false;
		}

		if(objectCore->ball->getPosition().y < 0)
		{
			play = false;
			objectCore->pad->setReleaseBall(false);
		}
		if(!play)
		{
			if(objectCore->pad->getReleaseBall())
			{
				Vec3 dir = objectCore->pad->getDirection();
				objectCore->ball->setDirection(dir.x, dir.y, dir.z);
				
				play = true;
				objectCore->pad->setReleaseBall(false);
			}

			objectCore->ball->setPosition(objectCore->pad->getBallPos());
			Vec3 temp = objectCore->ball->getPosition();
			//
			objectCore->ball->setPosition(Logic::from2DToCylinder(temp, 100, Vec3(150, 0, 0)));
			objectCore->ball->updateWorld();
			objectCore->ball->setPosition(temp);
		}
		if(GetAsyncKeyState(VK_NUMPAD0) != 0)
		{
			nextMap();
		}
		if(objectCore->bricks.size() == 0)
		{
			nextMap();
		}

		Vec3 padPos = objectCore->pad->getPosition();

		/////
		padPos.y += 100;
		padPos = Logic::from2DToCylinder(padPos, 100 + 150, Vec3(150, 0, 0));

		camera->setPosition(Vec3(padPos.x, padPos.y, padPos.z));
		//camera->setLookAt(Vec3(padPos.x, padPos.y, padPos.z));

		camera->update();
		
		// check collision between a ball and the bricks, will return the id of any brick the ball has
		// collided with, if no collision then -1 is returned
		int collidingObject = Logic::Check2DCollissions(objectCore->ball, objectCore->bricks);
		if(collidingObject != -1)
		{
			SAFE_DELETE(objectCore->bricks.at(collidingObject));
			objectCore->bricks.erase(objectCore->bricks.begin() + collidingObject, objectCore->bricks.begin() + collidingObject + 1);
			//std::cout << "Collided with a brick yo! Only " << objectCore->bricks.size() << " left!!!!" << std::endl;
		}

	}
	void Gameplay::nextMap()
	{
		int noMaps = Resources::LoadHandler::getInstance()->getMapSize();
		currentMapIndex++;
		if(currentMapIndex >= noMaps)
			currentMapIndex = 0;

		std::cout << "switched to map with index: " << currentMapIndex << std::endl;
		mapLoading->loadMap(currentMapIndex, &objectCore->bricks,NULL,NULL);
		play = false;

		for(int c = 0; c < objectCore->bricks.size(); c++)
		{
			if(c % 4 == 0)
			{
				objectCore->bricks.at(c)->setWidth(20);
				objectCore->bricks.at(c)->setHeight(10);
			}
			else if(c % 4 == 1)
			{
				objectCore->bricks.at(c)->setWidth(15);
				objectCore->bricks.at(c)->setHeight(7.5);
			}
			else if(c % 4 == 2)
			{
				objectCore->bricks.at(c)->setWidth(25);
				objectCore->bricks.at(c)->setHeight(5);
			}
			objectCore->bricks.at(c)->transformToCyl();
		}
	}
	Gameplay::~Gameplay()
	{
		SAFE_DELETE(camera);
		SAFE_DELETE(objectCore);
	}
}
