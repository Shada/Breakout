#ifndef _BRICK_H_
#define _BRICK_H_


#include "Object3D.h"

namespace Logic
{
	class Brick: public Object3D
	{
	public:
		Brick(Vec3 _pos);
		~Brick();
		void update(double _dt);

		//Possible parameters, like damage dealt and/or effects and such
		virtual void damage(); //These 2 could be defined here, or in their respective classes in case they were to function differently
		virtual void destroy();
		
	protected:
		int health;
		bool alive;
	};
}
#endif // ! _BRICK_H_