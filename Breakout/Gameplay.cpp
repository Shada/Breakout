#include "Gameplay.h"

#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#include <iostream>
#endif

namespace Logic
{
	int Gameplay::startEffect = 0;
	Gameplay::Gameplay(Inputhandler *&_handler, SoundSystem *soundSys)
	{
		
		mapLoading = new Map();
		//tolka Map och skBAJSAPA object enligt den
		objectCore = new ObjectCore();
		play = ballPadCollided = false;

		soundSystem = soundSys;
		eventSystem = new EventSystem(0,5); // testvärde
		srand (time(NULL));

		playerLives = 3;

		effectStart = 0;
		startEffectOld = 0;
		effectTypeActive = 0;

		#ifdef _WIN32
		GraphicsDX11::getInstance()->setObjectCore(objectCore);
		#else
		GraphicsOGL4::getInstance()->setObjectCore(objectCore);
		#endif
		objectCore->mapType = objectCore->MapType::eWater;

		objectCore->ball->setModelID(0);
		camera = new Camera();

		camera->setPosition(Logic::fitToScreen(Vec3(0,200,0), Vec3(300,200,0), Vec3(0,0,0), Vec3(300,0,0)));
		//camera->setPosition(Logic::fitToScreen(Vec3(0,200,0), Vec3(200,200,0), Vec3(0,0,0), Vec3(200,0,0)));
		Logic::calculateCameraBorders(camera->getPosition(), -camera->getPosition().z, (float)(4.f / 3));

		std::vector<KeyBind> keys;
		keys.push_back(KeyBind(KC_UP, &objectCore->pad->rotateLeft));
		keys.push_back(KeyBind(KC_DOWN, &objectCore->pad->rotateRight));
		keys.push_back(KeyBind(KC_LEFT, &objectCore->pad->moveLeft));
		keys.push_back(KeyBind(KC_RIGHT, &objectCore->pad->moveRight));
		keys.push_back(KeyBind(KC_NUMPAD9, &StartEffectReset));
		keys.push_back(KeyBind(KC_NUMPAD1, &StartEffectZapper));
		keys.push_back(KeyBind(KC_NUMPAD2, &StartEffectWind));
		keys.push_back(KeyBind(KC_NUMPAD3, &StartEffectFireballs));
		keys.push_back(KeyBind(KC_NUMPAD4, &StartEffectEarthquake));
		keys.push_back(KeyBind(KC_NUMPAD5, &StartEffectSpeed));
		keys.push_back(KeyBind(KC_NUMPAD6, &StartEffectSlow));
		keys.push_back(KeyBind(KC_NUMPAD7, &StartEffectStun));

		_handler->setPad(objectCore->pad, keys);
		//inputHandler = handler;

		//inputHandler->setCamera(camera, keys);



		currentMapIndex = 0;
		mapLoading->loadMap(currentMapIndex, &objectCore->bricks, objectCore->ball, objectCore->pad);
		if(objectCore->mapType == objectCore->MapType::eWater)
			objectCore->water = new Water(objectCore->pad->getPosition().y);

		#ifndef _WIN32
		GraphicsOGL4::getInstance()->initVertexBuffer();
		#endif

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

		/*if(objectCore->ball->getPosition().y < 0)
		{
			play = false;
			objectCore->pad->setReleaseBall(false);
		}*/

		if (play == true && objectCore->ball->getPosition().y < objectCore->pad->getPosition().y - 15)
		{
			play = false;
			objectCore->pad->setReleaseBall(false);
			playerLives--;
			std::cout << "Life lost! Nr of lives left: " << playerLives << std::endl;
			if (playerLives <= 0)
				nextMap(); //Replace with game over stuff
		}

		if(!play)
		{
			if(objectCore->pad->getReleaseBall())
			{
				Vec3 dir = objectCore->pad->getDirection();
				objectCore->ball->setDirection(dir.x, dir.y, NULL);
				
				play = true;
				objectCore->pad->setReleaseBall(false);
			}

			objectCore->ball->setPosition(objectCore->pad->getBallPos());
			objectCore->ball->updateWorld();
		}
#ifdef _WIN32
		if(GetAsyncKeyState(VK_NUMPAD0) != 0)
		{
			nextMap();
		}
#endif
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
		if(startEffectOld != startEffect)
		{
			effectStart = startEffect;
			startEffectOld = startEffect;
		}

		if (minorEffects.size() != 0) 
			for(int i = minorEffects.size(); i > 0; i--)
			{
				minorEffects[i-1].pos.y += -_dt * 20;
				if ((objectCore->pad->getPosition() - minorEffects[i-1].pos).length() < 10)
				{
					if(minorEffects[i-1].type == 0) //Lifegain
					{
						playerLives++;
						std::cout << "Life gained! Lives left :" << playerLives << std::endl;
						minorEffects.erase(minorEffects.begin() + i -1);
					}
					else if(minorEffects[i-1].type == 1) //Speedbuff
					{
						std::cout << "Speedbuff caught" << playerLives << std::endl;
						objectCore->pad->startSpeed();
						soundSystem->Play(16);
						minorEffects.erase(minorEffects.begin() + i -1);
					}
					else if(minorEffects[i-1].type == 2) //SpeedDebuff
					{
						std::cout << "Speed Debuff caught" << playerLives << std::endl;
						objectCore->pad->startSlow();
						soundSystem->Play(17);
						minorEffects.erase(minorEffects.begin() + i -1);
					}
					else if(minorEffects[i-1].type == 3) //Inverted Controls
					{}
					else if(minorEffects[i-1].type == 4) //Rotation speed changed
					{}
				}
				else if(minorEffects[i-1].pos.y < objectCore->pad->getPosition().y - 20)
					minorEffects.erase(minorEffects.begin() + i -1);
			}


		//if(play)
		//if (effectStart == 0)
		//	effectStart = eventSystem->Update(_dt);

		if (effectStart != 0 && effectTypeActive == 0)//Start av effekter
		{
			#pragma region effects
			
			effectStart = 14; //TEST
			std::cout << "effect started: ";
			if (effectStart == 1) //Zapper
			{
				//starta förvanande effekt
				effectTypeActive = 1;
				effectTimer = 1;
				effectOriginal = objectCore->pad->getPosition();
				std::cout << "Zapper" << std::endl;
			}
			else if (effectStart == 2) //Wind
			{
				effectTypeActive = 2;
				effectTimer = 1;
				soundSystem->Play(12, 0);
				std::cout << "Wind" << std::endl;
			}
			else if (effectStart == 4) //Fireballs
			{
				effectTypeActive = 4;
				effectTimer = 3;
				effectSpawnTimer = 0;
				Vec3 tempVec3;
				tempVec3 = Vec3((float)(rand()% 50), 200, 0);
				effectFireballs.push_back(tempVec3);
				soundSystem->Play(13, 3);
				std::cout << "Fireballs" << std::endl;
			}
			else if (effectStart == 5)//Earthquake
			{
				objectCore->pad->startSlow();
				effectTypeActive = 5;
				effectOriginal = camera->getPosition();
				effectTimer = 3.5;
				effectDirection = Vec3(((float)(rand()%100)-50), (float)(rand()%100-50), (float)(rand()%100)-50);
				soundSystem->Play(18, 1);
				std::cout << "Earthquake" << std::endl;
			}
			else if (effectStart == 7)//Speed
			{
				objectCore->pad->startSpeed();
				soundSystem->Play(16);
				std::cout << "Speed" << std::endl;
			}
			else if (effectStart == 8)//Slow
			{
				objectCore->pad->startSlow();
				soundSystem->Play(17);
				std::cout << "Slow" << std::endl;
			}
			else if (effectStart == 14)//Extra Life
			{
				//Lägg till grafik
				MinorEffect tempEff;
				tempEff.type = 0;
				tempEff.pos = Vec3((float)(rand()% 50), (float)(rand()% 150 + 50), 0);
				minorEffects.push_back(tempEff);
				std::cout << "Extra Life at X: " << tempEff.pos.x << " Y:" << tempEff.pos.y << std::endl;
			}
			else if (effectStart == 15)//Stun
			{
				objectCore->pad->startStun();
				soundSystem->Play(6);
				std::cout << "Stun" << std::endl;
			}
			effectStart = 0;
			#pragma endregion 
		}
		if(effectTypeActive != 0)//Uppdatering av aktiv effekt
		{
			#pragma region activeEffects
			effectTimer -= _dt;
			if (effectTypeActive == 1)//Zapper
			{
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
			else if (effectTypeActive == 2)//Wind
			{
				if (effectTimer < 0)
				{
					objectCore->ball->startWind();
					effectTypeActive = 0;
				}
			}
			else if (effectTypeActive == 4)//Fireballs
			{
				if (effectTimer > 0)
				{
					effectSpawnTimer += _dt;
					Vec3 tempVec3;
					if (rand() % 300 - effectSpawnTimer * 10 <= 1 && effectFireballs.size() <= 5)
					{
						tempVec3 = Vec3((float)(rand()% 200), 200, 0);
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
						playerLives--;
						std::cout << "Fireball hit! Life left: " << playerLives << std::endl;
					}
					else
						std::cout << "Fireball miss" << std::endl;
				}
				if (effectFireballs.size() == 0)
					effectTypeActive = 0;
			}
			else if (effectTypeActive == 5)//Earthquake
			{
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
						effectDirection = Vec3((float)(rand()%120-60), (float)(rand()%120-60), (float)(rand()%120-60));
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
		playerLives = 3;
		play = false;
	}
	Gameplay::~Gameplay()
	{
		SAFE_DELETE(eventSystem);
		SAFE_DELETE(camera);
		SAFE_DELETE(objectCore);
	}
}
