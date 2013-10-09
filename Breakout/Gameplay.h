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
#include "Water.h"
namespace Logic
{
	
	//GameState gameState;
	class Gameplay
	{
	private:
		int fps;
		enum MapType
		{
			eTest,
			eWind,
			eWater,
			eFire,
			eEarth
		};
		MapType mapType;
		std::vector<Vertex>			verticesDynamic;
		std::vector<PerInstance>	verticesPerInstance;

		int currentMapIndex;
		Map *mapLoading;
		Water *water;
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