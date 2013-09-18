#include "Timer.h"


Timer::Timer()
{
	deltaTime = 0;
	oldTime = clock();
}
	
void Timer::Tick()
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
	deltaTime = difftime((time_t)newTime, (time_t)oldTime); //(double)(newTime - oldTime);
	deltaTime /= (double)CLOCKS_PER_SEC; //Transform to seconds instead of nr of clocks.
	oldTime = newTime;
}
	
double Timer::getDelta()
{
	return deltaTime;
}
	
int Timer::getFPS()
{
	//Lazy, but works.
	return (int)(1.0 / deltaTime + .5);
}