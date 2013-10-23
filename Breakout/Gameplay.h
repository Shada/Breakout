#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

#include "Model.h"
#include "Map.h"
//#include "Pad.h"
#include "Inputhandler.h"
#include "Camera.h"
#include "Physics.h"
#include "Resource.h"
#include "ObjectCore.h"
#include "EventSystem.h"
#include "SoundSystem.hpp"
#include <cstdlib>
#include <time.h>

namespace Logic
{
	
	//GameState gameState;
	class Gameplay
	{
	private:
		std::vector<Vertex>			verticesDynamic;
		std::vector<PerInstance>	verticesPerInstance;

		int currentMapIndex;
		Map *mapLoading;
		Physics *physics;
		/* called after all models are initialized */
		
		ObjectCore *objectCore;
		bool play, ballPadCollided, createBall;
		Camera *camera;

		//TODO:
		//All logik för spelandet av en bana
		SoundSystem *soundSystem;
		EventSystem *eventSystem;
		
		int playerScore;
		int playerLives, fps, prevFps;
		
		Inputhandler *inputHandler;

		static int startEffect;
		int effectStart, startEffectOld;
		int effectTypeActive;
		Vec3 effectDirection, effectOriginal;
		float effectTimer, effectSpawnTimer;

		std::vector<Vec3> effectFireballs;
		std::vector<MinorEffect> minorEffects;

		void nextMap();
		void doubleBallEffect();
		void setMaptype(int type);
		void spawnEffect(int brickID, int i, int type);
		void reset();

	public:
		Gameplay(Inputhandler *&handler,SoundSystem *soundSys, ObjectCore *objectCore);
		~Gameplay();

		void setFps(int _fps) { fps = _fps; }
		void update(double dt);
		
		//You can not play the same effect twice in a row unless you
		//call StartEffectReset() by pressing key 9
		static void StartEffectReset()			{startEffect = 0;}
		static void StartEffectZapper()			{startEffect = 1;}
		static void StartEffectWind()			{startEffect = 2;}
		static void StartEffectFireballs()		{startEffect = 4;}
		static void StartEffectEarthquake()		{startEffect = 5;}
		static void StartEffectSpeed()			{startEffect = 7;}
		static void StartEffectSlow()			{startEffect = 8;}
		static void StartEffectStun()			{startEffect = 15;}
		
	};

}
#endif // ! _GAMEPLAY_H_