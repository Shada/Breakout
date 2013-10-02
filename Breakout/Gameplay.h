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

namespace Logic
{
	//GameState gameState;
	class Gameplay
	{
	private:
		std::vector<Vertex>			verticesDynamic;
		std::vector<PerInstance>	verticesPerInstance;

		Map *mapLoading;

		/* called after all models are initialized */
		
		ObjectCore *objectCore;
		bool play;
		Camera *camera;


		SoundSystem *soundSystem;
		EventSystem *eventSystem;


	public:
		Gameplay(Inputhandler *&handler,SoundSystem *soundSys);
		~Gameplay();

		void update(double dt);
	};

}
#endif // ! _GAMEPLAY_H_