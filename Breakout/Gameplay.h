#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

#include "Model.h"
#include "Resource.h"
namespace Logic
{
	class Gameplay
	{
	public:
		Gameplay(/*Map*/);
		~Gameplay();


	private:
		std::vector<Vertex>			verticesDynamic;
		std::vector<PerInstance>	verticesPerInstance;

		std::vector<Model*>			models;
		/* called after all models are initialized */
		void initVertexBuffer();
		//Brick bricks[];
		//Pad pad;
		//Ball balls[]
		
		//TODO:
		//All logik för spelandet av en bana
		
		int Update(/*tid*/);


	};

}
#endif // ! _GAMEPLAY_H_