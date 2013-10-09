#ifndef _SOUNDSYSTEM_H_
#define _SOUNDSYSTEM_H_

#include <string>
//#include "stdafx.h"
#include "Sound.h"

class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();
	void Initialize();
	void Update(double dt);
	void Play(int soundNr);
	void Play(int soundNr, double lifeTime);
	void PlayLoop(int soundNr);
	void PlayLoop(int soundNr, double lifeTime);
	void FadeLoop(int soundNr);
	void StopSound(int soundNr);

	FMOD::System *fmodSystem;
	
private:
	FMOD_RESULT  result;
	Sound sound[20]; 
		//
		//sound[0] = Meny background music
		//sound[1] = Windlevel background music
		//sound[2] = Waterlevel background music
		//sound[3] = Firelevel background music
		//sound[4] = Earthlevel background music
		//sound[5] = Game Over trombone
		//sound[6] = Lightning
		//sound[7] = Brick hit 1 (2 lifes left)
		//sound[8] = Brick hit 2 (1 life left)
		//sound[9] = Brick destroyed
		//sound[10] = Pad hit
		//sound[11] = Waves
		//sound[12] = Wind
		//sound[13] = Eruption (fire)
		//sound[14] = Fire balls
		//sound[15] = Bumpers
		//sound[16] = Positive effect activated
		//sound[17] = Negative effect activated
		//sound[18] = Earthquake
		//sound[]FINNS EJ ÄN = namn
		//sound[]FINNS EJ ÄN = namn

#ifdef BAJSAPA
#else
	//FMOD_LINUX_EXTRADRIVERDATA linuxExtraDriverData; //TODO Hitta rätt data till denna
#endif

};


#endif