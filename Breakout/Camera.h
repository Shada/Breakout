#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Resource.h"
#include "linearalgebra.h"

//TODO: move back to .cpp, bastard
#include "GraphicsOGL4.h"

class Camera
{

private:
	Vec3	position,
			rotation;

	Matrix	viewMatrix,
            viewInv,
			projectionMatrix,
			projectionInv;


public:
	Camera();
	~Camera();

	void setPosition(Vec3 position);
	void setRotation(Vec3 rotation);
	void setYPos(float _y)				{	position.y = _y; }; //For use in waterworld.

	Vec3 getPosition();
	Vec3 getRotation();
	Matrix getViewMatrix();

	void update();
};

#endif // ! _CAMERA_H_
