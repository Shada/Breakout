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
		objectCore->mapType = objectCore->MapType::eWater;

		objectCore->ball->setModelID(0);
		camera = new Camera();

		camera->setPosition(Logic::fitToScreen(Vec3(0,200,0), Vec3(300,200,0), Vec3(0,0,0), Vec3(300,0,0)));
		Logic::calculateCameraBorders(camera->getPosition(), -camera->getPosition().z, (float)(4.f / 3));

		std::vector<KeyBind> keys;
		keys.push_back(KeyBind(KC_UP, &objectCore->pad->rotateLeft));
		keys.push_back(KeyBind(KC_DOWN, &objectCore->pad->rotateRight));
		keys.push_back(KeyBind(KC_LEFT, &objectCore->pad->moveLeft));
		keys.push_back(KeyBind(KC_RIGHT, &objectCore->pad->moveRight));
		keys.push_back(KeyBind(KC_SPACE, &objectCore->pad->ejectBall));

		_handler->setPad(objectCore->pad, keys);
		//inputHandler = handler;

		//inputHandler->setCamera(camera, keys);

		currentMapIndex = 0;
		mapLoading->loadMap(currentMapIndex,&objectCore->bricks,objectCore->ball,objectCore->pad);
		if(objectCore->mapType == objectCore->MapType::eWater)
			objectCore->water = new Water(objectCore->pad->getPosition().y);

		
	}

	void Gameplay::update(double _dt)
	{
		objectCore->pad->update(_dt);
		if(play)
		{
			objectCore->ball->update(_dt);
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
				objectCore->ball->setDirection(dir.x, dir.y, 0);
				
				play = true;
				objectCore->pad->setReleaseBall(false);
			}

			objectCore->ball->setPosition(objectCore->pad->getBallPos());
			objectCore->ball->updateWorld();
		}
		if(GetAsyncKeyState(VK_NUMPAD0) != 0)
		{
			nextMap();
		}
		if(objectCore->bricks.size() == 0)
		{
			nextMap();
		}
		if(objectCore->mapType == objectCore->MapType::eWater)
		{
			objectCore->water->update(_dt);
			Vec3 oldPos = camera->getPosition();
			Vec3 oldLookat = camera->getLookAt();
			// should be the pad that follows water level and then camera follows pad?
			camera->setPosition(Vec3(oldPos.x, objectCore->water->getWaterLevel(),oldPos.z));
			camera->setLookAt(Vec3(oldLookat.x,objectCore->water->getWaterLevel(),oldLookat.z));
			Logic::calculateCameraBorders(camera->getPosition(), -camera->getPosition().z, (float)(4.f / 3));
			oldPos = objectCore->pad->getPosition();
			objectCore->pad->setPosition(Vec3(oldPos.x,objectCore->water->getWaterLevel(),oldPos.z));
		}
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

		/*static float diff = 0.0f;
		diff += 0.5f * _dt;
		objectCore->pad->setPosition(Logic::from2DToCylinder(objectCore->pad->getPosition(), 105, diff, Vec3(105,0,0)));*/
	}
	void Gameplay::nextMap()
	{
		int noMaps = Resources::LoadHandler::getInstance()->getMapSize();
		currentMapIndex++;
		if(currentMapIndex >= noMaps)
			currentMapIndex = 0;

		std::cout << "switched to map with index: " << currentMapIndex << std::endl;
		mapLoading->loadMap(currentMapIndex, &objectCore->bricks,NULL,NULL);
		if(objectCore->mapType == objectCore->MapType::eWater)
		{
			SAFE_DELETE(objectCore->water);
			objectCore->water = new Water(objectCore->pad->getPosition().y);
		}
		play = false;
	}
	Gameplay::~Gameplay()
	{
		SAFE_DELETE(camera);
		SAFE_DELETE(objectCore);
	}
}
