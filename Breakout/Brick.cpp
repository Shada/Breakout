#include "Brick.h"
#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#endif // _WIN32

namespace Logic
{
	Brick::Brick(Vec3 _pos)
	{
		position = _pos;
		scale		= Vec3(1,1,1);
		rotation	= Vec3(0,0,0);

		alive = true;

		#ifdef _WIN32
		shaderTechniqueID = GraphicsDX11::getInstance()->getTechIDByName("techSimple");
		#endif
		updateWorld();
	}

	Brick::~Brick()
	{

	}

	void Brick::update(double _dt)
	{

	}

	void Brick::damage()
	{
		health--;
		if(health < 0) this->destroy();
	}

	void Brick::destroy()
	{
		//remove hitbox;
		alive = false;
	}
}