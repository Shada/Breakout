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
		keys.push_back(KeyBind(DIK_UPARROW, &objectCore->pad->rotateLeft));
		keys.push_back(KeyBind(DIK_DOWNARROW, &objectCore->pad->rotateRight));
		keys.push_back(KeyBind(DIK_LEFTARROW, &objectCore->pad->moveLeft));
		keys.push_back(KeyBind(DIK_RIGHTARROW, &objectCore->pad->moveRight));

		_handler->setPad(objectCore->pad, keys);
		//inputHandler = handler;
		
		//inputHandler->setCamera(camera, keys);

		mapLoading->loadMap(0,&objectCore->bricks,objectCore->ball,objectCore->pad);

		objectCore->uiBillboards.push_back(BBUI());
		objectCore->uiBillboards.at(objectCore->uiBillboards.size() - 1).pos = Vec2(500,500);
		objectCore->uiBillboards.at(objectCore->uiBillboards.size() - 1).rotation = 0;
		objectCore->uiBillboards.at(objectCore->uiBillboards.size() - 1).size = Vec2(1024,768);
		objectCore->uiBillboards.at(objectCore->uiBillboards.size() - 1).texIndex = 0;
		objectCore->uiBillboards.at(objectCore->uiBillboards.size() - 1).tintAlpha = Vec4(0,0,0,1);
	}

	void Gameplay::update(double _dt)
	{
		objectCore->pad->update(_dt);
		objectCore->ball->update(_dt);
		camera->update();

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