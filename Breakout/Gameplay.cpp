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
		keys.push_back(KeyBind(DIK_LEFTARROW, &objectCore->pad->moveRight));
		keys.push_back(KeyBind(DIK_RIGHTARROW, &objectCore->pad->moveRight));

		_handler->setPad(objectCore->pad, keys);
		//inputHandler = handler;
		
		//inputHandler->setCamera(camera, keys);

		objectCore->bricks.push_back(new Brick(Vec3(100, 150, 0)));
		objectCore->bricks.push_back(new Brick(Vec3(170, 150, 0)));
		objectCore->bricks.push_back(new Brick(Vec3(240, 150, 0)));
		objectCore->bricks.push_back(new Brick(Vec3(310, 150, 0)));
		objectCore->bricks.push_back(new Brick(Vec3(380, 150, 0)));
		objectCore->bricks.push_back(new Brick(Vec3(450, 150, 0)));
		objectCore->bricks.push_back(new Brick(Vec3(520, 150, 0)));
		objectCore->bricks.push_back(new Brick(Vec3(100, 200, 0)));
		objectCore->bricks.push_back(new Brick(Vec3(170, 200, 0)));
		objectCore->bricks.push_back(new Brick(Vec3(240, 200, 0)));
		objectCore->bricks.push_back(new Brick(Vec3(310, 200, 0)));
		objectCore->bricks.push_back(new Brick(Vec3(380, 200, 0)));
		objectCore->bricks.push_back(new Brick(Vec3(450, 200, 0)));
		objectCore->bricks.push_back(new Brick(Vec3(520, 200, 0)));

		for(int i = 0; i < Resources::LoadHandler::getInstance()->getModelSize(); i++)
			models.push_back(Resources::LoadHandler::getInstance()->getModel(i));

		initVertexBuffer();
	}

	void Gameplay::initVertexBuffer()
	{
		std::vector<Vertex> vertices;
		int start = vertices.size();
		for(unsigned int i = 0; i < models.size(); i++)
		{
			models[i]->setStartIndex(start);
			vertices.insert(vertices.end(), models[i]->getData()->begin(), models[i]->getData()->end());
			start += models[i]->getData()->size();
		}

	#ifdef _WIN32
		GraphicsDX11::getInstance()->createVBufferStatic(vertices);
	#else
		//linux stuff
	#endif // _WIN32
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

	void Gameplay::draw()
	{
		//Resources::LoadHandler::getInstance()->getModel(ball->getModelID())->draw();
		objectCore->ball->draw();

		for(unsigned int i = 0; i < objectCore->bricks.size(); i++)
		{
			
		}
	}

	Gameplay::~Gameplay()
	{
		SAFE_DELETE(camera);
		
		models.clear();
	}
}