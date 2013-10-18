#include "Gameplay.h"

#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#include <iostream>
#endif

namespace Logic
{
	Gameplay::Gameplay(Inputhandler *&_handler, SoundSystem *soundSys)
	{		
		mapLoading = new Map();
		inputHandler = _handler;

		objectCore = new ObjectCore();
		play = ballPadCollided = createBall = false;

		soundSystem = soundSys;
		eventSystem = new EventSystem(0,5); // testv�rde
		srand ((unsigned)time(NULL));

		#ifdef _WIN32
		GraphicsDX11::getInstance()->setObjectCore(objectCore);
		#else
		GraphicsOGL4::getInstance()->setObjectCore(objectCore);
		#endif

		Vec3 vec1 = Logic::from2DToCylinder(Vec3(0,0,0), 100);
		Vec3 vec2 = Logic::from2DToCylinder(Vec3(75,0,0), 100);
		Vec3 vec3 = Logic::from2DToCylinder(Vec3(150,0,0), 100);
		int c = 0; c++;


		this->setMaptype(objectCore->MapType::eFire);

		objectCore->ball.at(0)->setModelID(0);
		camera = new Camera();
	/*	Logic::sph2Cart(Vec3(0,1.570796,39));
		Logic::cart2Sph(Vec3(39,0,0));*/

		//camera->setPosition(Logic::fitToScreen(Vec3(-384,256,0), Vec3(384,256,0), Vec3(-384,0,0), Vec3(384,0,0)));
		camera->setPosition(Logic::fitToScreen(Vec3(0,200,0), Vec3(300,200,0), Vec3(0,0,0), Vec3(300,0,0)));
		Logic::calculateCameraBorders(camera->getPosition(), -camera->getPosition().z, (float)(4.f / 3));

		
		//inputHandler = handler;
		std::vector<KeyBind> keys;
		keys.push_back(KeyBind(KC_UP, &objectCore->pad->rotateLeft));
		keys.push_back(KeyBind(KC_DOWN, &objectCore->pad->rotateRight));
		keys.push_back(KeyBind(KC_SPACE, &objectCore->pad->ejectBall));
		keys.push_back(KeyBind(KC_LEFT, &objectCore->pad->moveLeft));
		keys.push_back(KeyBind(KC_RIGHT, &objectCore->pad->moveRight));

		inputHandler->setPad(objectCore->pad, keys);


		//inputHandler->setCamera(camera, keys);



		currentMapIndex = 0;

		mapLoading->loadMap(currentMapIndex, &objectCore->bricks, objectCore->ball.at(0), objectCore->pad);
		this->setMaptype(mapLoading->getMapType());
		if(objectCore->getMapType() == objectCore->MapType::eWater)

			objectCore->water = new Water(objectCore->pad->getPosition().y);


		#ifndef _WIN32
		GraphicsOGL4::getInstance()->initVertexBuffer();
		#endif
	}

	void Gameplay::update(float _dt)
	{
		static bool isPressed = false;

		if(objectCore->getMapType() == objectCore->MapType::eFire)
		{
			objectCore->pad->updateCylinder(_dt);

			Vec3 padPos = objectCore->pad->getPosition();
			padPos.y += 100;
			padPos = Logic::from2DToCylinder(padPos, 100 + 150, Vec3(150, 0, 0));

			camera->setPosition(Vec3(padPos.x, padPos.y, padPos.z));
		}
		else
			objectCore->pad->update(_dt);

		if(play)
		{

			if(objectCore->getMapType() == objectCore->MapType::eFire)
				for(unsigned int i = 0; i < objectCore->ball.size(); i++)
					objectCore->ball.at(i)->updateCylinder(_dt);
			else
				for(unsigned int i = 0; i < objectCore->ball.size(); i++)
					objectCore->ball.at(i)->update(_dt);
			if(!ballPadCollided)
				for(unsigned int i = 0; i < objectCore->ball.size(); i++)
					ballPadCollided = Logic::ballCollision(objectCore->ball.at(i), objectCore->pad, objectCore->pad->getRotation().z);
			else
				ballPadCollided = false;
		}

		if(objectCore->ball.size() == 1 && objectCore->ball.at(0)->getPosition().y < 0)
		{
			play = false;
			objectCore->pad->setReleaseBall(false);
		}
		else
		{
			for(unsigned int i = objectCore->ball.size() - 1; i > 0; i--)
				if(objectCore->ball.at(i)->getPosition().y < 0)
				{
					SAFE_DELETE(objectCore->ball.at(i));
					objectCore->ball.erase(objectCore->ball.begin() + i, objectCore->ball.begin() + i + 1);
				}
		}

		if(!play)
		{
			if(objectCore->pad->getReleaseBall())
			{
				Vec3 dir = objectCore->pad->getDirection();
				objectCore->ball.at(0)->setDirection(dir.x, dir.y, dir.z);
				
				play = true;
				objectCore->pad->setReleaseBall(false);
			}

			objectCore->ball.at(0)->setPosition(objectCore->pad->getBallPos());
			//Vec3 temp = objectCore->ball->getPosition();
			//objectCore->ball->setPosition(temp);

			if(objectCore->getMapType() == objectCore->MapType::eFire)
				objectCore->ball.at(0)->transformToCyl();
			else
				objectCore->ball.at(0)->updateWorld();
			//objectCore->ball->setPosition(temp);
		}
#ifdef _WIN32
		if(GetAsyncKeyState(VK_NUMPAD0) != 0 && !isPressed)
		{
			nextMap();
			this->setMaptype(mapLoading->getMapType());
		}
		if(play && GetAsyncKeyState(VK_NUMPAD5) != 0 && !createBall)

			doubleBallEffect();

		else if(createBall && GetAsyncKeyState(VK_NUMPAD5) == 0)
			createBall = false;
#endif
		if(objectCore->bricks.size() == 0)
		{
			nextMap();
			this->setMaptype(mapLoading->getMapType());
		}

		//Vec3 padPos = objectCore->pad->getPosition();

		///////
		//padPos.y += 100;
		//padPos = Logic::from2DToCylinder(padPos, 100 + 150, Vec3(150, 0, 0));

		if(objectCore->getMapType() == objectCore->MapType::eWater)
		{
			objectCore->water->update(_dt);
			Vec3 oldPos = camera->getPosition();
			Vec3 oldLookat = camera->getLookAt();
			// should be the pad that follows water level and then camera follows pad?
			camera->setPosition(Vec3(oldPos.x, objectCore->water->getWaterLevel(),oldPos.z));
			camera->setLookAt(Vec3(oldLookat.x,objectCore->water->getWaterLevel(),oldLookat.z));
			Logic::calculateCameraBorders(camera->getPosition(), -camera->getPosition().z, 4.f / 3);
			oldPos = objectCore->pad->getPosition();
			objectCore->pad->setPosition(Vec3(oldPos.x,objectCore->water->getWaterLevel(),oldPos.z));
		}

		camera->update();
		
		// check collision between a ball and the bricks, will return the id of any brick the ball has
		// collided with, if no collision then -1 is returned

		if(!play)
			return;

		for(unsigned int i = 0; i < objectCore->ball.size(); i++)
		{
			int collidingObject = Logic::Check2DCollissions(objectCore->ball.at(i), objectCore->bricks, objectCore->getMapType() == objectCore->MapType::eFire);
			if(collidingObject != -1)
			{
				Brick *tempBrick = dynamic_cast<Brick *>(objectCore->bricks.at(collidingObject));
				tempBrick->damage();
				if(tempBrick->isDestroyed() == true)
				{
					SAFE_DELETE(objectCore->bricks.at(collidingObject));
					objectCore->bricks.erase(objectCore->bricks.begin() + collidingObject, objectCore->bricks.begin() + collidingObject + 1);
					std::cout << "Collided with a brick yo! Only " << objectCore->bricks.size() << " left!!!!" << std::endl;
					if(rand() % 100 < 5)
						doubleBallEffect();
				}
				//else
					//std::cout << "Collided with a brick yo! But it is still alive!" << std::endl;
			}
		}

		//Effects
		//if(play)
		int temptest = eventSystem->Update(_dt);
		if (temptest != 0)//Start av effekter
		{
			#pragma region effects
			temptest = 0; //TEST
			std::cout << "effect started: ";
			if (temptest == 1) //Zapper
			{
				//starta f�rvanande effekt
				effectTypeActive = 1;
				effectTimer = 1;
				effectOriginal = objectCore->pad->getPosition();
				std::cout << "Zapper" << std::endl;
			}
			else if (temptest == 2) //Wind
			{
				//objectCore->ball->startWind();
				soundSystem->Play(12, 0.5);
				std::cout << "Wind" << std::endl;
			}
			else if (temptest == 4) //Fireballs
			{
				effectTypeActive = 4;
				effectTimer = 3;
				effectSpawnTimer = 0;
				Vec3 tempVec3;
				tempVec3 = Vec3((float)(rand() % 50), 200, 0);
				effectFireballs.push_back(tempVec3);
				soundSystem->Play(13, 3);
				std::cout << "Fireballs" << std::endl;
			}
			else if (temptest == 5)//Earthquake
			{
				objectCore->pad->startSlow();
				effectTypeActive = 5;
				effectOriginal = camera->getPosition();
				effectTimer = 3.5;
				effectDirection = Vec3((float)(rand() % 100) - 50, (float)(rand() % 100) - 50, (float)(rand() % 100) - 50);
				soundSystem->Play(18, 1);
				std::cout << "Earthquake" << std::endl;
			}
			else if (temptest == 7)//Speed
			{
				objectCore->pad->startSpeed();
				soundSystem->Play(16);
				std::cout << "Speed" << std::endl;
			}
			else if (temptest == 8)//Slow
			{
				objectCore->pad->startSlow();
				soundSystem->Play(17);
				std::cout << "Slow" << std::endl;
			}
			else if (temptest == 15)//Stun
			{
				objectCore->pad->startStun();
				soundSystem->Play(6);
				std::cout << "Stun" << std::endl;
			}
			#pragma endregion 
		}
		if(effectTypeActive != 0)//Uppdatering av aktiv effekt
		{
			#pragma region activeEffects
			if (effectTypeActive == 1)//Zapper
			{
				effectTimer -= _dt;
				if (effectTimer < 0)
				{
					soundSystem->Play(6);
					effectTimer = 0;
					effectTypeActive = 0;
					effectOriginal -= objectCore->pad->getPosition();
					
					std::cout << effectOriginal.length();
					if(effectOriginal.length() < 20)
					{
						objectCore->pad->startStun();
						std::cout << "Zapper hit" << std::endl;
					}
					else
						std::cout << "Zapper miss" << std::endl;
				}
			}
			else if (effectTypeActive == 4)//Fireballs
			{
				if (effectTimer > 0)
				{
					effectTimer -= _dt;
					effectSpawnTimer += _dt;
					Vec3 tempVec3;
					if (rand() % 300 - effectSpawnTimer * 10 <= 1 && effectFireballs.size() <= 5)
					{
						tempVec3 = Vec3((float)(rand() % 200), 200, 0);
						effectFireballs.push_back(tempVec3);
						effectSpawnTimer = 0;
					}
				}

				for(unsigned int i = 0; i < effectFireballs.size(); i++)
					effectFireballs[i].y += -_dt * 60;
				
				//objectCore->pad->setPosition(effectFireballs[0]);//TEST

				if (effectFireballs[0].y < 45)
				{
					effectOriginal = effectFireballs[0] - objectCore->pad->getPosition();
						
					effectFireballs.erase(effectFireballs.begin());
					if (effectOriginal.length() < 20)
					{
						objectCore->pad->startStun();
						std::cout << "Fireball hit" << std::endl;
					}
					else
						std::cout << "Fireball miss" << std::endl;
				}

				if (effectFireballs.size() == 0)
					effectTypeActive = 0;
			}
			else if (effectTypeActive == 5)//Earthquake
			{
				effectTimer -= _dt;
				Vec3 tempVec;
				tempVec = camera->getPosition();
				
				if (effectTimer < 1)
				{
					tempVec = Vec3(tempVec.x * (1 -_dt*10) + _dt*10 * effectOriginal.x,
									tempVec.y * (1 -_dt*10) + _dt*10 * effectOriginal.y,
									tempVec.z * (1 -_dt*10) + _dt*10 * effectOriginal.z);
					camera->setPosition(tempVec);
				}
				else
				{
					if (rand()%100 <= 20)
						effectDirection = Vec3((float)(rand() % 120) - 60, (float)(rand() % 120) - 60, (float)(rand() % 120) - 60);
					tempVec = Vec3(tempVec.x + _dt * effectDirection.x,
									tempVec.y + _dt * effectDirection.y,
									tempVec.z + _dt * effectDirection.z);
					camera->setPosition(tempVec);
				}

				if (effectTimer < 0)
				{
					effectTimer = 0;
					effectTypeActive = 0;
					camera->setPosition(effectOriginal);
				}
			}
			#pragma endregion 
		}


		isPressed = GetAsyncKeyState(VK_NUMPAD0);
	}

	void Gameplay::nextMap()
	{
		int noMaps = Resources::LoadHandler::getInstance()->getMapSize();
		currentMapIndex++;
		if(currentMapIndex >= noMaps)
			currentMapIndex = 0;

		std::cout << "switched to map with index: " << currentMapIndex << std::endl;
		mapLoading->loadMap(currentMapIndex, &objectCore->bricks,NULL,NULL);
		if(objectCore->getMapType() == objectCore->MapType::eWater)
		{
			SAFE_DELETE(objectCore->water);
			objectCore->water = new Water(objectCore->pad->getPosition().y);
		}

		if(objectCore->ball.size() > 1)
			for(unsigned int i = objectCore->ball.size() - 1; i > 0; i--)
			{
				SAFE_DELETE(objectCore->ball.at(i));
				objectCore->ball.erase(objectCore->ball.begin() + i, objectCore->ball.begin() + i + 1);
			}

		play = false;
	}


	void Gameplay::setMaptype(int _type)
	{
		
		objectCore->setMapType(_type);
	}

	void Gameplay::doubleBallEffect()
	{
		createBall = true;
		int ballSize = objectCore->ball.size();
		for(int i = 0; i < ballSize; i++)
		{
			if(objectCore->ball.size() == 8)
				break;
			objectCore->ball.push_back(new Ball());
			objectCore->ball.back()->setPosition(objectCore->ball.at(i)->getPosition());
			objectCore->ball.back()->setDirection((rand() % 100) - 200, (rand() % 100) - 200, 0);
			objectCore->ball.back()->setModelID(2);
		}
		std::cout << "BAAAAAALLSS " << ballSize * 2 << std::endl;
	}

	Gameplay::~Gameplay()
	{
		SAFE_DELETE(eventSystem);
		SAFE_DELETE(camera);
		SAFE_DELETE(objectCore);
	}
}
