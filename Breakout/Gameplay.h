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
		//Brick bricks[];
		//Pad pad;
		//Ball balls[]
		ObjectCore *objectCore;
		bool play, ballPadCollided;
		Camera *camera;
		//TODO:
		//All logik för spelandet av en bana
		void nextMap();
	public:
		Gameplay(Inputhandler *&handler);
		~Gameplay();

		void update(double dt);
	};

}
#endif // ! _GAMEPLAY_H_