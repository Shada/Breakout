#pragma once
#include "SoundSystem.hpp"
//#include "DSPfilters.h"


SoundSystem::SoundSystem()
{
/*
#ifdef _WIN32
#else
	//linuxExtraDriverData = new FMOD_LINUX_EXTRADRIVERDATA
#endif
*/
}

SoundSystem::~SoundSystem()
{
	//for(int i = 0; i < 18; i++)
	
	//radera Sounds först
	result = fmodSystem->close();
    result = fmodSystem->release();
}

void SoundSystem::Initialize()
{
	result = FMOD::System_Create(&fmodSystem);

#ifdef _WIN32
	result = fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
#else
	result = fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
	//result = fmodSystem->init(32, FMOD_INIT_NORMAL, linuxExtraDriverData); //linuxExtraDriverData är tom för tillfället!
#endif
	
	sound[0].Initialize(fmodSystem, 0.5, "Sounds/destination.mp3");
	sound[1].Initialize(fmodSystem, 0.5, "Sounds/midnight-ride.mp3");
	sound[2].Initialize(fmodSystem, 0.5, "Sounds/urban-spy-2.mp3");
	sound[3].Initialize(fmodSystem, 0.5, "Sounds/barn-beat.mp3");
	sound[4].Initialize(fmodSystem, 0.5, "Sounds/power-juice.mp3");
	sound[5].Initialize(fmodSystem, 0.9, "Sounds/fail-trombone-01.mp3");
	sound[6].Initialize(fmodSystem, 0.9, "Sounds/Explosion large_BLASTWAVEFX_31097.mp3");
	sound[7].Initialize(fmodSystem, 1.0, "Sounds/switch-7.mp3");
	sound[8].Initialize(fmodSystem, 1.0, "Sounds/switch-5.mp3");
	sound[9].Initialize(fmodSystem, 0.8, "Sounds/sound4.mp3");
	sound[10].Initialize(fmodSystem, 1.0, "Sounds/Plink_06.mp3");
	sound[11].Initialize(fmodSystem, 0.7, "Sounds/lake-waves-01.mp3");
	sound[12].Initialize(fmodSystem, 0.8, "Sounds/wind-howl-01.mp3");
	sound[13].Initialize(fmodSystem, 1.0, "Sounds/fire-1.mp3");
	sound[14].Initialize(fmodSystem, 0.7, "Sounds/explosion-03.mp3");
	sound[15].Initialize(fmodSystem, 1.0, "Sounds/sound97.wav");
	sound[16].Initialize(fmodSystem, 1.0, "Sounds/sound99.wav");
	sound[17].Initialize(fmodSystem, 1.0, "Sounds/sound95.wav");

}

void SoundSystem::Play(int soundNr)
{
	sound[soundNr].Play();
}

void SoundSystem::Play(int soundNr, double lifeTime)
{
	sound[soundNr].Play(lifeTime);
}

void SoundSystem::PlayLoop(int soundNr)
{
	sound[soundNr].PlayLoop();
}

void SoundSystem::PlayLoop(int soundNr, double lifeTime)
{
	sound[soundNr].PlayLoop(lifeTime);
}

void SoundSystem::StopSound(int soundNr)
{
	sound[soundNr].Stop();
}

void SoundSystem::FadeLoop(int soundNr)
{
	sound[soundNr].FadeLoop();
}

void SoundSystem::Update(double dt)
{

	//Uppdatera alla ljud som behöver time
	fmodSystem->update();
	for(int i = 0; i <= 17; i++) // ordna rätt nummer när alla ljud är inne
		sound[i].Update(dt);
}

