#ifndef _TIMER_H_
#define _TIMER_H_

#include <time.h>

class Timer
{
	
private:
	double oldTime, newTime;
	double deltaTime;

public:
	Timer();
	void Tick();
	double getDelta();
	int getFPS();

};
#endif // ! _TIMER_H_