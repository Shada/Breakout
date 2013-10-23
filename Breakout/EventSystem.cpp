#include "EventSystem.h"
#include <cstdlib>
#include <time.h>

namespace Logic
{
	EventSystem::EventSystem(int typeOfMap, int difficulty)
	{
		srand((unsigned)time(NULL));
		mapType = typeOfMap;
		leveldifficulty = difficulty;
		nextSpawn = rand() % 20 + 10;
		spawnTimer = 0;
	}

	EventSystem::~EventSystem()
	{
	}

	int EventSystem::Update(double dt)
	{
		spawnTimer += dt;
		if (spawnTimer + (spawnTimer * leveldifficulty * 0.1) >= nextSpawn)
		{
			spawnTimer = 0;
			nextSpawn = rand() % 20 + 10;
			int tempType = rand() % 100;
			switch (mapType)
			{
			case 0: // 0 = testing
				if(tempType <= 10)
					return 1;
				else if(tempType <= 20)
					return 7;
				else if(tempType <= 30)
					return 8;
				else if(tempType <= 55)
					return 5;
				else if(tempType <= 65)
					return 4;
				else
					return 2;

			case 1: //1 = Wind
				if(tempType <= 40)
					return 1;
				else
					return 2;

			case 2: //2 = Water
				if(tempType <= 20)
					return 1;
				else if(tempType <= 65)
					return 2;
				else
					return 0;

			case 3: //3 = Fire
				if(tempType <= 35)
					return 4;
				else if(tempType <= 85)
					return 5;
				else
					return 0;

			case 4: //4= Earth
				if(tempType <= 50)
					return 5;
				else if(tempType <= 55)
					return 5;
				else
					return 0;
			}
		}
		return 0;
	}
}