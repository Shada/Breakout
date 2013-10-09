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
		//tolka Map och skapa object enligt den
		objectCore = new ObjectCore();
		play = false;

		soundSystem = soundSys;
		eventSystem = new EventSystem(0,5); // testvärde
		srand (time(NULL));

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
			Brick *tempBrick = dynamic_cast<Brick *>(objectCore->bricks.at(collidingObject));
			tempBrick->damage();
			if(tempBrick->isDestroyed() == true)
			{
				SAFE_DELETE(objectCore->bricks.at(collidingObject));
				objectCore->bricks.erase(objectCore->bricks.begin() + collidingObject, objectCore->bricks.begin() + collidingObject + 1);
				std::cout << "Collided with a brick yo! Only " << objectCore->bricks.size() << " left!!!!" << std::endl;
			}
			else
				std::cout << "Collided with a brick yo! But it is still alive!" << std::endl;
		}

		//Effects
		//if(play)
		int temptest = eventSystem->Update(_dt);
		if (temptest != 0)//Start av effekter
		{
			#pragma region effects
			//temptest = 4; //TEST
			std::cout << "effect started: ";
			if (temptest == 1) //Zapper
			{
				//starta förvanande effekt
				effectTypeActive = 1;
				effectTimer = 1;
				effectOriginal = objectCore->pad->getPosition();
				std::cout << "Zapper" << std::endl;
			}
			else if (temptest == 2) //Wind
			{
				objectCore->ball->startWind();
				soundSystem->Play(12, 0.5);
				std::cout << "Wind" << std::endl;
			}
			else if (temptest == 4) //Fireballs
			{
				effectTypeActive = 4;
				effectTimer = 3;
				effectSpawnTimer = 0;
				Vec3 tempVec3;
				tempVec3 = Vec3(rand()% 50, 200, 0);
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
				effectDirection = Vec3((rand()%100)-50, (rand()%100)-50, (rand()%100)-50);
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
						tempVec3 = Vec3(rand()% 200, 200, 0);
						effectFireballs.push_back(tempVec3);
						effectSpawnTimer = 0;
					}
				}

				for(int i = 0; i < effectFireballs.size(); i++)
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
						effectDirection = Vec3((rand()%120)-60, (rand()%120)-60, (rand()%120)-60);
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
	}

	Gameplay::~Gameplay()
	{
		SAFE_DELETE(eventSystem);
		SAFE_DELETE(camera);
		SAFE_DELETE(objectCore);
	}
}
