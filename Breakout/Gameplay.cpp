#include "Gameplay.h"
#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#include <iostream>
#endif

namespace Logic
{

	static float maxX = 960;
	static float maxY = 600;
	static Vec2 leftUI, rightUI;

	int Gameplay::startEffect = 0;
	Gameplay::Gameplay(Inputhandler *&_handler/**, SoundSystem *soundSys*/, ObjectCore *objectCore)
	{
		fps = 0, prevFps = -1;
		mapLoading = new Map();
		inputHandler = _handler;
		physics = Logic::Physics::getInstance();

		this->objectCore = objectCore;
		play = ballPadCollided = createBall = false;

		///soundSystem = soundSys;
		srand ((unsigned)time(NULL));

		playerLives = 3;
		playerScore = 0;

		effectStart = 0;
		startEffectOld = 0;
		effectTypeActive = 0;

		#ifdef _WIN32
		GraphicsDX11::getInstance()->setObjectCore(objectCore);
		#else
		GraphicsOGL4::getInstance()->setObjectCore(objectCore);
		#endif

		leftUI = Vec2(400 * (maxX/SCRWIDTH), SCRHEIGHT);

		this->setMaptype(ObjectCore::MapType::eWind);

		objectCore->ball.at(0)->setModelID(0);
		camera = new Camera();
	/*	Logic::sph2Cart(Vec3(0,1.570796,39));
		Logic::cart2Sph(Vec3(39,0,0));*/

		//camera->setPosition(Logic::fitToScreen(Vec3(0,360,0), Vec3(660,360,0), Vec3(0,0,0), Vec3(660,0,0)));
		camera->setPosition(physics->fitToScreen(Vec3(0 - leftUI.x, maxY, 0), Vec3(maxX - leftUI.x, maxY, 0), Vec3(0 - leftUI.x, 0, 0), Vec3(maxX - leftUI.x, 0, 0), maxX/maxY));
		Vec3 lookAt = camera->getPosition();
		lookAt.z = -lookAt.z;
		camera->setLookAt(lookAt);
		physics->calculateCameraBorders(camera->getPosition(), -camera->getPosition().z, maxX/maxY);


		//inputHandler = handler;
		std::vector<KeyBind> keys;
		keys.push_back(KeyBind(KC_UP, &objectCore->pad->rotateLeft));
		keys.push_back(KeyBind(KC_DOWN, &objectCore->pad->rotateRight));
		keys.push_back(KeyBind(KC_LEFT, &objectCore->pad->moveLeft));
		keys.push_back(KeyBind(KC_RIGHT, &objectCore->pad->moveRight));
		keys.push_back(KeyBind(KC_SPACE, &objectCore->pad->ejectBall));
		keys.push_back(KeyBind(KC_NUMPAD9, &StartEffectReset));
		keys.push_back(KeyBind(KC_NUMPAD1, &StartEffectZapper));
		keys.push_back(KeyBind(KC_NUMPAD2, &StartEffectWind));
		keys.push_back(KeyBind(KC_NUMPAD3, &StartEffectFireballs));
		keys.push_back(KeyBind(KC_NUMPAD4, &StartEffectEarthquake));
		keys.push_back(KeyBind(KC_NUMPAD5, &StartEffectSpeed));
		keys.push_back(KeyBind(KC_NUMPAD6, &StartEffectSlow));
		keys.push_back(KeyBind(KC_NUMPAD7, &StartEffectStun));

		_handler->setPad(objectCore->pad, keys);
		//inputHandler->setCamera(camera, keys);

		leftUI.x = 400;

		physics->setBorderMaxX(physics->getBorderX() - (300 * (maxX/SCRWIDTH)));

		objectCore->testFont->loadFontSettings("Fonts/blackwhite.txt");

		std::vector<BBFont> test = std::vector<BBFont>();
		objectCore->testFont->setImageIndex(7);
		objectCore->testText->setFont(objectCore->testFont);
		objectCore->testText->setTextData(0, 10);

		currentMapIndex = 0;

		mapLoading->loadMap(currentMapIndex, &objectCore->bricks, objectCore->ball.at(0), objectCore->pad, &objectCore->mapType);

		objectCore->mapType = ObjectCore::MapType::eWater;// test
		if(objectCore->mapType == ObjectCore::MapType::eWater)
			objectCore->water = new Water(objectCore->pad->getPosition().y,0);
		if(objectCore->mapType == ObjectCore::MapType::eFire)
			objectCore->water = new Water(objectCore->pad->getPosition().y,1);

///		soundSystem->PlayLoop(5);

		eventSystem = new EventSystem(mapLoading->getMapType(),mapLoading->getLvlDifficulty()); // testvärde

		#ifndef _WIN32
		GraphicsOGL4::getInstance()->initVertexBuffer();
		GraphicsOGL4::getInstance()->feedUIBufferData();
		GraphicsOGL4::getInstance()->feedTextBufferData();
		#endif

		objectCore->sideBar->setTexIndex(objectCore->getMapType()-1);
	}
float _time = 1;
	void Gameplay::update(double _dt)
	{
		float dt = (float)_dt;
		_time += dt;
		if(_time > 20) /// && objectCore->mapType != ObjectCore::MapType::eFire) // switching map every 30 seconds
		{
            nextMap();
            _time = 0;
		}
		Vec3 cameratem = camera->getLookAt();
		//fps = (int)(1.0 / _dt + 0.5);

		//update label
		if(prevFps != fps)
		{
			std::ostringstream buffFps;
			buffFps << fps;
			std::string fpsText = "FPS: " + buffFps.str();
			objectCore->testText->setText( fpsText.c_str() );
			objectCore->testText->updateTextData();

			prevFps = fps;
		}

		static bool isPressed = false;

		if(objectCore->getMapType() == ObjectCore::MapType::eFire)
		{
			objectCore->pad->updateCylinder(_dt);
			objectCore->water->update(_dt);
			Vec3 oldPos = objectCore->pad->getPosition();
			objectCore->pad->setPosition(Vec3(oldPos.x,objectCore->water->getWaterLevel(),oldPos.z));

			Vec3 padPos = objectCore->pad->getPosition();
			padPos.y += 50;

			padPos = physics->from2DToCylinder(padPos, (float)(physics->getCylRadius() + 150), Vec3((float)physics->getBorderX() / 2, 0, 0));
			if(effectTypeActive == 5)
				camera->setPosition(Vec3(padPos.x + effectOriginal.x, padPos.y + effectOriginal.y, padPos.z + effectOriginal.z));
			else
				camera->setPosition(Vec3(padPos.x, padPos.y, padPos.z));
			camera->setLookAt(Vec3 (physics->getBorderX()/2, 50 + objectCore->water->getWaterLevel() * 0.4f, 0));
		}
		else
			objectCore->pad->update(_dt);


		if(play)
		{
			if(objectCore->getMapType() == ObjectCore::MapType::eFire)
				for(unsigned int i = 0; i < objectCore->ball.size(); i++)
					objectCore->ball.at(i)->updateCylinder(_dt);
			else
				for(unsigned int i = 0; i < objectCore->ball.size(); i++)
					objectCore->ball.at(i)->update(_dt);
			if(!ballPadCollided)
				for(unsigned int i = 0; i < objectCore->ball.size(); i++)
					ballPadCollided = physics->ballCollision(objectCore->ball.at(i), objectCore->pad, objectCore->pad->getRotation().z);
			else
				ballPadCollided = false;
		}

		if(objectCore->ball.size() == 1 && objectCore->ball.at(0)->getPosition().y < 0)
		{
			play = false;
			objectCore->pad->setReleaseBall(false);
			playerLives--;
			std::cout << "Life lost! Nr of lives left: " << playerLives << std::endl;

			for(unsigned int i = 0; i < objectCore->effects.size(); i++)
				SAFE_DELETE(objectCore->effects.at(i));
			objectCore->effects.clear();

			if (playerLives <= 0)
			{
///				soundSystem->Play(6);
				nextMap(); //Replace with game over stuff
			}
		}
		else
		{
			for(unsigned int i = 0; i < objectCore->ball.size(); i++)
				if(objectCore->ball.at(i)->getPosition().y < 0)
				{
					SAFE_DELETE(objectCore->ball.at(i));
					objectCore->ball.erase(objectCore->ball.begin() + i, objectCore->ball.begin() + i + 1);
					i--;
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

			if(objectCore->getMapType() == ObjectCore::MapType::eFire)
				objectCore->ball.at(0)->transformToCyl();
			else
				objectCore->ball.at(0)->updateWorld();
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

		if(GetAsyncKeyState(VK_NUMPAD9) != 0)
			objectCore->pad->invertControls(2);

		if(GetAsyncKeyState(VK_NUMPAD7) != 0)
			objectCore->pad->decreaseRotation(2);

		if(GetAsyncKeyState(VK_NUMPAD1) != 0)
			std::cout<<"X: "<<objectCore->pad->getPosition().x<<". Y: "<<objectCore->pad->getPosition().y<<std::endl;

		if(GetAsyncKeyState(VK_NUMPAD2) != 0)
			std::cout<<"X: "<<objectCore->ball.at(0)->getPosition().x<<". Y: "<<objectCore->ball.at(0)->getPosition().y<<std::endl;
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

		if(objectCore->getMapType() == ObjectCore::MapType::eWater)
		{
			objectCore->water->update(_dt);
			Vec3 oldPos = camera->getPosition();
			Vec3 oldLookat = camera->getLookAt();
			float waterLevel = objectCore->water->getWaterLevel();
			// should be the pad that follows water level and then camera follows pad?

			camera->setPosition(Vec3(oldPos.x, waterLevel + 75,oldPos.z));
			camera->setLookAt(Vec3(oldPos.x, waterLevel + 25, oldPos.z + 10000));
			camera->setWaterLevel(waterLevel);
			physics->setBorderMaxY(maxY / 2 + waterLevel + 75);
			//camera->setPosition(Vec3(oldPos.x, objectCore->water->getWaterLevel(),oldPos.z));
			//camera->setLookAt(Vec3(oldLookat.x,objectCore->water->getWaterLevel(),oldLookat.z));
			//Logic::calculateCameraBorders(camera->getPosition(), -camera->getPosition().z, 4.f / 3);

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
			int collidingObject = physics->Check2DCollissions(objectCore->ball.at(i), objectCore->bricks, objectCore->getMapType() == ObjectCore::MapType::eFire);
			if(collidingObject != -1)
			{
				Brick *tempBrick = dynamic_cast<Brick *>(objectCore->bricks.at(collidingObject));
				tempBrick->damage();
				if(tempBrick->isDestroyed() == true)
				{
					//if(rand() % 100 < 50)
					{
						int type;
						int effectType = rand() % 6;
						if	   (effectType < 1) type = 0;
						else if(effectType < 2) type = 1;
						else if(effectType < 3) type = 2;
						else if(effectType < 4) type = 3;
						else if(effectType < 5) type = 4;
						else					type = 5;
						//doubleBallEffect();
						spawnEffect(collidingObject, i, type);
					}
					SAFE_DELETE(objectCore->bricks.at(collidingObject));
					objectCore->bricks.erase(objectCore->bricks.begin() + collidingObject, objectCore->bricks.begin() + collidingObject + 1);
					std::cout << "Collided with a brick yo! Only " << objectCore->bricks.size() << " left!!!!" << std::endl;

					playerScore += 1;
					std::cout << "Score: " << playerScore << std::endl;

				}
				//else
					//std::cout << "Collided with a brick yo! But it is still alive!" << std::endl;
			}
		}

		if(objectCore->effects.size() > 0)
			for(unsigned int i = 0; i < objectCore->effects.size(); i++)
			{
				objectCore->effects.at(i)->update(_dt);
				if(objectCore->effects.at(i)->getPosition().y < 0)
				{
					SAFE_DELETE(objectCore->effects.at(i));
					objectCore->effects.erase(objectCore->effects.begin() + i, objectCore->effects.begin() + i + 1);
					i--;
					continue;
				}
				if((objectCore->pad->getPosition() - objectCore->effects.at(i)->getPosition()).length() < 10)
				{
					int type = objectCore->effects.at(i)->getType();
					switch(type)
					{
					case 0:		playerLives++;							break;
					case 1:		objectCore->pad->startSpeed();			break;
					case 2:		objectCore->pad->startSlow();			break;
					case 3:		objectCore->pad->invertControls(2.f);	break;
					case 4:		objectCore->pad->decreaseRotation(2.f);	break;
					case 5:		doubleBallEffect();
					}
					SAFE_DELETE(objectCore->effects.at(i));
					objectCore->effects.erase(objectCore->effects.begin() + i, objectCore->effects.begin() + i + 1);
					i--;
				}
			}


		//Effects
		if(startEffectOld != startEffect)
		{
			effectStart = startEffect;
			startEffectOld = startEffect;
		}

		objectCore->testText->update( _dt );

		//if(play)
		if (effectStart == 0 && effectTypeActive == 0)
			effectStart = eventSystem->Update(_dt);

		if (effectStart != 0 && effectTypeActive == 0)//Start av effekter
		{
			#pragma region effects
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
///				soundSystem->Play(13, 0);
				for(unsigned int i = 0; i < objectCore->ball.size(); i++)
					objectCore->ball.at(i)->startWind();
				std::cout << "Wind" << std::endl;
			}
			else if (effectStart == 4) //Fireballs
			{
				effectTypeActive = 4;
				effectTimer = 3;
				effectSpawnTimer = 0;
				Vec3 tempVec3;
				tempVec3 = Vec3((float)(rand() % 50), 200, 0);
				effectFireballs.push_back(tempVec3);
///				soundSystem->Play(14, 3);
				std::cout << "Fireballs" << std::endl;
			}
			else if (effectStart == 5)//Earthquake
			{
				objectCore->pad->startSlow();
				effectTypeActive = 5;
				//effectOriginal = camera->getPosition();
				effectOriginal = Vec3(0,0,0);
				effectTimer = 3.5;
				effectDirection = Vec3((float)(rand() % 100) - 50, (float)(rand() % 100) - 50, (float)(rand() % 100) - 50);
	///			soundSystem->Play(19, 1.5);
				std::cout << "Earthquake" << std::endl;
			}
			else if (effectStart == 7)//Speed
			{
				objectCore->pad->startSpeed();
	///			soundSystem->Play(17);
				std::cout << "Speed" << std::endl;
			}
			else if (effectStart == 8)//Slow
			{
				objectCore->pad->startSlow();
	///			soundSystem->Play(19);
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
		///		soundSystem->Play(7);
				std::cout << "Stun" << std::endl;
			}
			effectStart = 0;
			#pragma endregion
		}
		if(effectTypeActive != 0)//Uppdatering av aktiv effekt
		{
			#pragma region activeEffects
			effectTimer -= dt;
			if (effectTypeActive == 1)//Zapper
			{
				if (effectTimer < 0)
				{
		///			soundSystem->Play(7);
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
					for(unsigned int i = 0; i < objectCore->ball.size(); i++)
						objectCore->ball.at(i)->startWind();
					effectTypeActive = 0;
				}
			}
			else if (effectTypeActive == 4)//Fireballs
			{
				if (effectTimer > 0)
				{
					effectSpawnTimer += dt;
					Vec3 tempVec3;
					if (rand() % 300 - effectSpawnTimer * 10 <= 1 && effectFireballs.size() <= 5)
					{
						tempVec3 = Vec3((float)(rand() % 200), 200, 0);
						effectFireballs.push_back(tempVec3);
						effectSpawnTimer = 0;
					}
				}

				for(unsigned int i = 0; i < effectFireballs.size(); i++)
					effectFireballs[i].y += -dt * 60;

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
					effectOriginal = Vec3(effectOriginal.x * (1 -_dt*10),
										  effectOriginal.y * (1 -_dt*10),
										  effectOriginal.z * (1 -_dt*10));
					camera->setPosition(tempVec + effectOriginal);
				}
				else
				{
					if (rand()%100 <= 20)
						effectDirection = Vec3((float)(rand() % 120) - 60, (float)(rand() % 120) - 60, (float)(rand() % 120) - 60);
					effectOriginal = Vec3(effectOriginal.x + _dt * effectDirection.x,
										effectOriginal.y + _dt * effectDirection.y,
										effectOriginal.z + _dt * effectDirection.z);
					camera->setPosition(tempVec + effectOriginal);
				}

				if (effectTimer < 0)
				{
					effectTimer = 0;
					effectTypeActive = 0;
					//camera->setPosition(effectOriginal);
				}
			}
			#pragma endregion
		}


		//isPressed = GetAsyncKeyState(VK_NUMPAD0);
	}

	void Gameplay::nextMap()
	{

		std::cout << "Final score: " << playerScore << std::endl;
		int noMaps = Resources::LoadHandler::getInstance()->getMapSize();
		currentMapIndex++;
		if(currentMapIndex >= noMaps)
			currentMapIndex = 0;

		std::cout << "switched to map with index: " << currentMapIndex << std::endl;
		mapLoading->loadMap(currentMapIndex, &objectCore->bricks, NULL, objectCore->pad, &objectCore->mapType);

		if(objectCore->mapType == ObjectCore::MapType::eWater)
		{
			SAFE_DELETE(objectCore->water);
			objectCore->water = new Water(objectCore->pad->getPosition().y,0);
		}

		if(objectCore->mapType == ObjectCore::MapType::eFire)
		{
			SAFE_DELETE(objectCore->water);
			objectCore->water = new Water(objectCore->pad->getPosition().y,1);
		}
		objectCore->sideBar->setTexIndex(objectCore->getMapType()-1);

		reset();
	}

	void Gameplay::reset()
	{
		if(objectCore->mapType != ObjectCore::MapType::eFire)
		{
			leftUI = Vec2(400 * (maxX/SCRWIDTH), SCRHEIGHT);
			camera->setPosition(physics->fitToScreen(Vec3(0 - leftUI.x, maxY, 0), Vec3(maxX - leftUI.x, maxY, 0), Vec3(0 - leftUI.x, 0, 0), Vec3(maxX - leftUI.x, 0, 0), maxX/maxY));
			Vec3 lookAt = camera->getPosition();
			lookAt.z = -lookAt.z;
			camera->setLookAt(lookAt);
			physics->calculateCameraBorders(camera->getPosition(), -camera->getPosition().z, maxX/maxY);
			physics->setBorderMaxX(physics->getBorderX() - (300 * (maxX/SCRWIDTH)));
			objectCore->pad->setRotation(Vec3(0,0,0));
			leftUI = Vec2(400, SCRHEIGHT);
		}
		else
		{
			physics->setBorderMaxX(1300);
		}

		playerLives = 3;
		playerScore = 0;
		eventSystem->setTypeOfMap(mapLoading->getMapType());
		eventSystem->setDifficulty(mapLoading->getLvlDifficulty());

		if(objectCore->ball.size() > 1)
			for(unsigned int i = objectCore->ball.size() - 1; i > 0; i--)
			{
				SAFE_DELETE(objectCore->ball.at(i));
				objectCore->ball.erase(objectCore->ball.begin() + i, objectCore->ball.begin() + i + 1);
			}

		play = false;
		for(unsigned int i = 0; i < objectCore->effects.size(); i++)
			SAFE_DELETE(objectCore->effects.at(i));

		objectCore->effects.clear();
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
			objectCore->ball.back()->setDirection((float)((rand() % 200) - 100), (float)((rand() % 200) - 100), 0);
			objectCore->ball.back()->setModelID(2);
			objectCore->ball.back()->setTextureID(objectCore->ball.at(i)->getTextureID());
		}
	}

	void Gameplay::spawnEffect(int _brickID, int _i, int _type)
	{
		objectCore->effects.push_back(new Effect(objectCore->bricks.at(_brickID)->getPosition(), objectCore->ball.at(_i)->getDirection().y,
			_type, objectCore->mapType == ObjectCore::MapType::eFire));
	}

	Gameplay::~Gameplay()
	{
		SAFE_DELETE(eventSystem);
		SAFE_DELETE(camera);
		SAFE_DELETE(objectCore);
		//SAFE_DELETE(water);
		SAFE_DELETE(mapLoading);
		SAFE_DELETE(physics);
	}
}
