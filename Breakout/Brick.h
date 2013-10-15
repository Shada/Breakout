#ifndef _BRICK_H_
#define _BRICK_H_


#include "Object3D.h"

namespace Logic
{
	class Brick: public Object3D
	{
	public:
		Brick(Vec3 _pos);
		Brick(Vec3 _pos, double _width, double _height);
		~Brick();
		void update(double _dt);
		void draw();

		void setHeight( double _h);
		void setWidth( double _w);

		double getWidth() { return width; };
		double getHeight() { return height; };

		//Possible parameters, like damage dealt and/or effects and such
		virtual void damage(); //These 2 could be defined here, or in their respective classes in case they were to function differently
		virtual void destroy();
		
	protected:
		int health;
		bool alive;
		double width, height;

	private:
		int shaderTechniqueID;

	};
}
#endif // ! _BRICK_H_