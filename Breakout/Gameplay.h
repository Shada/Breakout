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
	class Gameplay
	{
	private:
		std::vector<Vertex>			verticesDynamic;
		std::vector<PerInstance>	verticesPerInstance;

		Map *mapLoading;

		/* called after all models are initialized */
		//Brick bricks[];
		//Pad pad;
		//Ball balls[]
		ObjectCore *objectCore;
		Camera *camera;
		//TODO:
		//All logik för spelandet av en bana

	public:
		Gameplay(Inputhandler *&handler);
		~Gameplay();

		void update(double dt);
		void draw();
	};

}
#endif // ! _GAMEPLAY_H_