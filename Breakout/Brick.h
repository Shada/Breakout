#ifndef _BRICK_H_
#define _BRICK_H_


#include "Object3D.h"

namespace Logic
{
	class Brick: public Object3D
	{
	public:
		Brick(Vec3 _pos, bool _cylinder);
		Brick(Vec3 _pos, bool _cylinder, double _width, double _height);
		~Brick();
		void update(double _dt);
		void draw();

		void setHeight( double _h);
		void setWidth( double _w);

		//Possible parameters, like damage dealt and/or effects and such
		virtual void damage(); //These 2 could be defined here, or in their respective classes in case they were to function differently
		virtual void destroy();

		void setType(int _Type);
		void setWidth(float _w)		{ width = _w; scale.x = _w / 5; updateWorld(); }
		void setHeight(float _h)	{ height = _h; scale.y = _h / 5; updateWorld(); }
		bool isDestroyed();

		float getWidth() { return width; }
		float getHeight() { return height; }

		
	protected:
		int health;
		bool alive;

	private:
		float height, width;
		int shaderTechniqueID;

	};
}
#endif // ! _BRICK_H_