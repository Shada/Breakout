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
	
	sound[0].Initialize(fmodSystem, 0.5f, "Sounds/destination.mp3");
	sound[1].Initialize(fmodSystem, 0.5f, "Sounds/midnight-ride.mp3");
	sound[2].Initialize(fmodSystem, 0.5f, "Sounds/urban-spy-2.mp3");
	sound[3].Initialize(fmodSystem, 0.5f, "Sounds/barn-beat.mp3");
	sound[4].Initialize(fmodSystem, 0.5f, "Sounds/power-juice.mp3");
	sound[5].Initialize(fmodSystem, 0.5f, "Sounds/toko2.mp3");
	sound[6].Initialize(fmodSystem, 0.9f, "Sounds/fail-trombone-01.mp3");
	sound[7].Initialize(fmodSystem, 0.9f, "Sounds/Explosion large_BLASTWAVEFX_31097.mp3");
	sound[8].Initialize(fmodSystem, 1.0f, "Sounds/switch-7.mp3");
	sound[9].Initialize(fmodSystem, 1.0f, "Sounds/switch-5.mp3");
	sound[10].Initialize(fmodSystem, 0.8f, "Sounds/sound4.mp3");
	sound[11].Initialize(fmodSystem, 1.0f, "Sounds/Plink_06.mp3");
	sound[12].Initialize(fmodSystem, 0.7f, "Sounds/lake-waves-01.mp3");
	sound[13].Initialize(fmodSystem, 0.8f, "Sounds/wind-howl-01.mp3");
	sound[14].Initialize(fmodSystem, 1.0f, "Sounds/fire-1.mp3");
	sound[15].Initialize(fmodSystem, 0.7f, "Sounds/explosion-03.mp3");
	sound[16].Initialize(fmodSystem, 1.0f, "Sounds/sound97.wav");
	sound[17].Initialize(fmodSystem, 1.0f, "Sounds/sound99.wav");
	sound[18].Initialize(fmodSystem, 1.0f, "Sounds/sound95.wav");
	sound[19].Initialize(fmodSystem, 1.0f, "Sounds/car-interior-1.mp3");

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
	for(int i = 0; i <= 18; i++) // ordna rätt nummer när alla ljud är inne
		sound[i].Update(dt);
}

