#ifndef _TIMER_H_
#define _TIMER_H_

#include <time.h>

namespace Logic
{
	class Timer
	{
	
	private:
		clock_t oldTime, newTime;
		double deltaTime;

	public:
		Timer();
		void Tick();
		double getDelta();
		int getFPS();

	};
}

#endif