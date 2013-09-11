#include "Timer.h"

Logic::Timer::Timer()
{
	deltaTime = 0;
	oldTime = clock();
}

void Logic::Timer::Tick()
{
	//Probably a pretty dirty solution...
	//if(deltaTime == NULL) //This check is done to prevent odd numbers at first tick (where oldTime is undefined).
	//{
	//	deltaTime = 0;
	//	oldTime = clock();
	//	return;
	//}

	//Update new time, calculate deltaTime and finally, set oldTime.
	newTime = clock();
	deltaTime = (double)(newTime - oldTime);
	oldTime = newTime;
}

double Logic::Timer::getDelta()
{
	return deltaTime;
}

int Logic::Timer::getFPS()
{
	//Lazy, but works.
	return (int)(1.0 / deltaTime);
}