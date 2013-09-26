#include "Brick.h"
#ifdef _WIN32
#include "GraphicsDX11.h"
#endif // _WIN32

namespace Logic
{
	Brick::Brick(Vec3 _pos)
	{
		position = _pos;
		alive = true;

		#ifdef _WIN32
		shaderTechniqueID = GraphicsDX11::getInstance()->getTechIDByName("techSimple");
		#endif
	}

	Brick::~Brick()
	{

	}

	void Brick::update(double _dt)
	{
		
	}

	void Brick::draw()
	{
		CBWorld cb;
		cb.world = scalingMatrix(Vec3(1.5f, 1, 1)) *  translationMatrix(position);
#ifdef _WIN32
		GraphicsDX11::getInstance()->useTechnique(shaderTechniqueID);
		GraphicsDX11::getInstance()->updateCBWorld(cb);
#endif // _WIN32
		Resources::LoadHandler::getInstance()->getModel(modelID)->draw();
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