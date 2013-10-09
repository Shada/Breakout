#include "Sound.h"

Sound::Sound()
{
	channel = 0;
	timeLeft = -1000;
	sound1 = 0;
}

Sound::~Sound()
{
}

void Sound::Initialize(FMOD::System *fmodsystem, float vol, char* filename)
{
	//L�gg eventuellt till fix p� path i Linux
	timeLeft = -1000;
	fmodSystem = fmodsystem;
	result = fmodSystem->createSound(filename, FMOD_DEFAULT, 0, &sound1);
	standardVolume = vol;

}

void Sound::Play()
{
	timeLeft = -1000;
	sound1->setMode(FMOD_DEFAULT);
	result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sound1, false, &channel);
	channel->setVolume(standardVolume);
}

void Sound::Play(double lifeTime)
{
	timeLeft = lifeTime;
	sound1->setMode(FMOD_DEFAULT);
	result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sound1, false, &channel);
	channel->setVolume(standardVolume);
}

void Sound::PlayLoop()
{
	timeLeft = -1000;
	sound1->setMode(FMOD_LOOP_NORMAL);
	result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sound1, false, &channel);
	channel->setVolume(standardVolume);
}

void Sound::PlayLoop(double lifeTime)
{
	timeLeft = lifeTime;
	sound1->setMode(FMOD_LOOP_NORMAL);
	result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sound1, false, &channel);
	channel->setVolume(standardVolume);
}

void Sound::FadeLoop()
{
	timeLeft = 0;
}

void Sound::Update(double dt)
{
	if(timeLeft != -1000)
	{
		timeLeft = timeLeft - dt;
		if(timeLeft <= 0) 
		{
			float temp;
			result = channel->getVolume(&temp);
			temp = temp - (float)dt * 0.15f; // ordna timing etc
			if (temp < 0)
				Stop();
			else
				result = channel->setVolume(temp);
		}
	}
}

void Sound::Stop()
{
	channel->stop();
	//channel->setPaused(true);
	//channel->setPosition(0,FMOD_TIMEUNIT_MS); 
}

