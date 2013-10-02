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
		objectCore = new ObjectCore();
		play = false;

		#ifdef _WIN32
		GraphicsDX11::getInstance()->setObjectCore(objectCore);
		#else
		GraphicsOGL4::getInstance()->setObjectCore(objectCore);
		#endif

		objectCore->ball->setModelID(0);
		camera = new Camera();

		std::vector<KeyBind> keys;
		keys.push_back(KeyBind(KC_UP, &objectCore->pad->rotateLeft));
		keys.push_back(KeyBind(KC_DOWN, &objectCore->pad->rotateRight));
		keys.push_back(KeyBind(KC_LEFT, &objectCore->pad->moveLeft));
		keys.push_back(KeyBind(KC_RIGHT, &objectCore->pad->moveRight));
		keys.push_back(KeyBind(KC_SPACE, &objectCore->pad->ejectBall));

		_handler->setPad(objectCore->pad, keys);
		//inputHandler = handler;

		//inputHandler->setCamera(camera, keys);

		mapLoading->loadMap(0,&objectCore->bricks,objectCore->ball,objectCore->pad);

		objectCore->uiBillboards.push_back(BBUI());
		objectCore->uiBillboards.at(objectCore->uiBillboards.size() - 1).pos = Vec2(0,0);
		objectCore->uiBillboards.at(objectCore->uiBillboards.size() - 1).rotation = 0;
		objectCore->uiBillboards.at(objectCore->uiBillboards.size() - 1).size = Vec2(100,100);
		objectCore->uiBillboards.at(objectCore->uiBillboards.size() - 1).texIndex = 0;
		objectCore->uiBillboards.at(objectCore->uiBillboards.size() - 1).tintAlpha = Vec4(0,0,0,1);

		objectCore->testFont->loadFontSettings("Fonts/blackwhite.txt");
		std::vector<BBFont> test = std::vector<BBFont>();
		objectCore->testFont->setImageIndex(7);
		objectCore->testText->setFont(objectCore->testFont);
		objectCore->testText->updateTextData();
	}

	void Gameplay::update(double _dt)
	{
		objectCore->pad->update(_dt);
		if(play)
			objectCore->ball->update(_dt);

		if(objectCore->ball->getPosition().y < 10)
		{
			play = false;
			objectCore->pad->setReleaseBall(false);
		}
		if(!play)
		{
			if(objectCore->pad->getReleaseBall())
			{
				Vec3 dir = objectCore->pad->getDirection();
				objectCore->ball->setDirection(dir.x, dir.y);
				
				play = true;
				objectCore->pad->setReleaseBall(false);
			}

			objectCore->ball->setPosition(objectCore->pad->getBallPos());
			objectCore->ball->updateWorld();
		}

		camera->update();
		
		Logic::ballCollision(objectCore->ball, objectCore->pad, objectCore->pad->getRotation().z);

		int collidingObject = Logic::Check2DCollissions(objectCore->ball, objectCore->bricks);
		if(collidingObject != -1)
		{
			SAFE_DELETE(objectCore->bricks.at(collidingObject));
			objectCore->bricks.erase(objectCore->bricks.begin() + collidingObject, objectCore->bricks.begin() + collidingObject + 1);
			std::cout << "Collided with a brick yo! Only " << objectCore->bricks.size() << " left!!!!" << std::endl;
		}
	}

	Gameplay::~Gameplay()
	{
		SAFE_DELETE(camera);
		SAFE_DELETE(objectCore);
	}
}
