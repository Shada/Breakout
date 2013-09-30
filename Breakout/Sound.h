#ifndef _SOUND_H_
#define _SOUND_H_

#include <fmod.hpp>
#ifdef BAJSAPA
#else
//#include <fmodlinux.h>
#endif


class Sound
{
public:
	Sound();
	~Sound();
	void Initialize(FMOD::System*, float vol, char*);
	void Play();
	void Play(double lifeTime);
	void PlayLoop();
	void PlayLoop(double lifeTime);
	void FadeLoop();
	void Stop();
	void Update(double dt);

private:
	FMOD::System *fmodSystem;
	FMOD::Sound *sound1;
	//FMOD::ChannelGroup *mastergroup;
    FMOD::Channel *channel;
	FMOD_RESULT  result;
	float standardVolume;

	double timeLeft;
};

#endif