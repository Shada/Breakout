#ifndef _MAP_H_
#define _MAP_H_

#include <vector>
#include "Ball.h"
#include "Pad.h"
#include "Brick.h"

namespace Logic
{
	class Map
	{
	public:

		Map(){};
		~Map(){};
		void loadMap(unsigned int _mapID,std::vector<Object3D*> *_bricks,Ball *_ball,Pad *_pad);


		//TODO:
		//funktion för inladdning av bana
	private:
		//TODO:
		//förvaring av bana.
		//etc

	};
}

#endif // ! _MAP_H_
