#include "EventSystem.h"
#include <cstdlib>
#include <time.h>

namespace Logic
{
	EventSystem::EventSystem(int typeOfMap, int difficulty)
	{
		srand (time(NULL));
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
		if (spawnTimer * leveldifficulty * 0.5 >= nextSpawn)
		{
			spawnTimer = 0;
			nextSpawn = rand() % 20 + 10;
			int tempType = rand() % 100;
			switch (mapType)
			{
			case 0: // 0 = testing
				if(tempType <= 10)
					return 7;
				else if(tempType <= 20)
					return 8;
				else if(tempType <= 45)
					return 15;
				else
					return 2;

			case 1: //1 = Wind
				if(tempType <= 10)
					return 1;
				else if(tempType <= 30)
					return 1;
				else
					return 1;

			case 2: //2 = Water
				if(tempType <= 10)
					return 1;
				else if(tempType <= 45)
					return 1;
				else
					return 1;

			case 3: //3 = Fire
				if(tempType <= 15)
					return 1;
				else if(tempType <= 35)
					return 1;
				else
					return 1;

			case 4: //4= Earth
				if(tempType <= 10)
					return 1;
				else if(tempType <= 45)
					return 1;
				else
					return 1;
			}
		}
		return 0;
	}
}