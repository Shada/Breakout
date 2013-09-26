#include "Gameplay.h"
#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#endif
namespace Logic
{
	Gameplay::Gameplay(Inputhandler *&_handler)
	{
		mapLoading = new Map();
		//tolka Map och skapa object enligt den
		objectCore = new ObjectCore();

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

		_handler->setPad(objectCore->pad, keys);
		//inputHandler = handler;
		
		//inputHandler->setCamera(camera, keys);

		mapLoading->loadMap(0,&objectCore->bricks,objectCore->ball,objectCore->pad);
	}

	void Gameplay::update(double _dt)
	{
		objectCore->pad->update(_dt);
		objectCore->ball->update(_dt);
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

	void Gameplay::draw()
	{
		//Resources::LoadHandler::getInstance()->getModel(ball->getModelID())->draw();
		objectCore->ball->draw();
		objectCore->pad->draw();

		for(unsigned int i = 0; i < objectCore->bricks.size(); i++)
		{
			objectCore->bricks.at(i)->draw();
		}
	}

	Gameplay::~Gameplay()
	{
		SAFE_DELETE(camera);
		SAFE_DELETE(objectCore);
	}
}