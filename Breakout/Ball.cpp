#include "Ball.h"
#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#endif // _WIN32

namespace Logic
{

	Ball::Ball() : Object3D()
	{
		position = Vec3(0, 0, 0);
		direction = Vec3(1, 1, 0);
		radius = 25;
		direction.normalize();
		speed = 500;

#ifdef _WIN32
		shaderTechniqueID = GraphicsDX11::getInstance()->getTechIDByName("techSimple");
#endif
	}

	Ball::~Ball()
	{
	}

	void Ball::update(double _dt)
	{
		//Check for buffs/debuffs here, and apply them
		position += direction * speed * (float)_dt;
		nextFrame = position + (direction * speed * (float)_dt);
	}
	void Ball::draw()
	{
#ifdef _WIN32
		CBWorld cb;
		cb.world = scalingMatrix(Vec3(10, 10, 10)) * translationMatrix(position);
		GraphicsDX11::getInstance()->useTechnique(shaderTechniqueID);
		GraphicsDX11::getInstance()->updateCBWorld(cb);
#endif // _WIN32
		Resources::LoadHandler::getInstance()->getModel(modelID)->draw();
	}

#ifdef _WIN32
	void Ball::setDirection(float _x, float _y, float _z)
	{
		if(_x != NULL) direction.x = _x;
		if(_y != NULL) direction.y = _y;
		if(_z != NULL) direction.z = _z;
	}
#else
    void Ball::setDirection(float _x, float _y, float _z)
    {
        direction.x = _x;
        direction.y = _y;
        direction.z = _z;
    }
#endif // _WIN32

}
