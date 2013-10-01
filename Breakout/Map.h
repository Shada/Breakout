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
		/*	load a map, ball and pad can be NULL if no balls or no pad are desired 
			for example when changing from another map */
		void loadMap(unsigned int _mapID,std::vector<Object3D*> *_bricks,Ball *_ball,Pad *_pad);


		//TODO:
		//funktion f�r inladdning av bana
	private:
		//TODO:
		//f�rvaring av bana.
		//etc

	};
}

#endif // ! _MAP_H_
