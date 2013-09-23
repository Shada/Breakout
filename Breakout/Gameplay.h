#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

#include "Model.h"
#include "Map.h"
#include "Pad.h"
#include "Inputhandler.h"
#include "Camera.h"
#include "Ball.h"
#include "Physics.h"
#include "Brick.h"
#include "Resource.h"
namespace Logic
{
	class Gameplay
	{
	private:
		std::vector<Vertex>			verticesDynamic;
		std::vector<PerInstance>	verticesPerInstance;

		std::vector<Model*>			models;
		/* called after all models are initialized */
		void initVertexBuffer();
		//Brick bricks[];
		//Pad pad;
		//Ball balls[]
		Pad *pad;
		std::vector<Object3D*> bricks;
		Ball *ball;
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