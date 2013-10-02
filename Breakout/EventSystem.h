#ifndef _EVENTSYSTEM_H_
#define _EVENTSYSTEM_H_

#include <cstdlib>
#include <time.h>

namespace Logic
{
	/*
	Update() return values: 
	0 = Nothing happens
	1 != Zapper
	2 = Wind
	3 != Eruptions
	4 != Fire Balls
	5 != Earthquake
	6 != Bumpers
	7 = Speed increase
	8 = Speed decrease
	9 != Padsize increase
	10 != Padsize decrease
	11 != Inverted controlls
	12 != Rotationspeed increase
	12 != Rotationspeed decrease
	14 != Extra liv
	15 = Stun
	*/
	class EventSystem
	{
	public:
		EventSystem(int typeOfMap, int difficulty);
		~EventSystem();
		int Update(double dt);

	private:
		int mapType;
		int leveldifficulty;
		int nextSpawn;
		double spawnTimer;

	};



}


#endif