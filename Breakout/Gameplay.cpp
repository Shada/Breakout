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
		keys.push_back(KeyBind(DIK_LEFTARROW, &pad->moveRight));
		keys.push_back(KeyBind(DIK_RIGHTARROW, &pad->moveRight));

		_handler->setPad(pad, keys);
		//inputHandler = handler;
		
		//inputHandler->setCamera(camera, keys);

		mapLoading->loadMap(0,&bricks,ball,pad);

		for(int i = 0; i < Resources::LoadHandler::getInstance()->getModelSize(); i++)
			models.push_back(Resources::LoadHandler::getInstance()->getModel(i));

		initVertexBuffer();

		std::vector<Vertex> *ballPoints = models.at(2)->getData();
		float xMax = -100, yMax = -100, xMin = 100, yMin = 100;
		for(int c = 0; c < ballPoints->size(); c++)
		{
			if(ballPoints->at(c).pos.x > xMax)
				xMax = ballPoints->at(c).pos.x;
			if(ballPoints->at(c).pos.y > yMax)
				yMax = ballPoints->at(c).pos.y;
			if(ballPoints->at(c).pos.x < xMin)
				xMin = ballPoints->at(c).pos.x;
			if(ballPoints->at(c).pos.y < yMin)
				yMin = ballPoints->at(c).pos.y;
		}
		
		float radius = (xMax - xMin) * 0.5f;
		float radiuss = (yMax - yMin) * 0.5f;
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