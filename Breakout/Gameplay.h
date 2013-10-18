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
		/* called after all models are initialized */
		
		ObjectCore *objectCore;
		bool play, ballPadCollided;
		Camera *camera;

		//TODO:
		//All logik för spelandet av en bana
		void nextMap();
		void setMaptype(int type);

		SoundSystem *soundSystem;
		EventSystem *eventSystem;
		Inputhandler *inputHandler;

		int effectTypeActive;
		Vec3 effectDirection, effectOriginal;
		float effectTimer, effectSpawnTimer;
		std::vector<Vec3> effectFireballs;

	public:
		Gameplay(Inputhandler *&handler,SoundSystem *soundSys);
		~Gameplay();

		void update(float dt);
	};

}
#endif // ! _GAMEPLAY_H_