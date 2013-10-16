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
	Vec3	lookAt;

	Matrix	viewMatrix,
            viewInv,
			viewRefl,
			projectionMatrix,
			projectionInv;

	float  waterLevel;
public:
	Camera();
	~Camera();

	void setPosition(Vec3 position);
	void setRotation(Vec3 rotation);
	void setLookAt(Vec3 _lookAt) { lookAt = _lookAt;}
	void setYPos(float _y)				{	position.y = _y; }; //For use in waterworld.
	void setWaterLevel(float _waterLevel){waterLevel = _waterLevel;}
	Vec3 getPosition();
	Vec3 getRotation();
	Vec3 getLookAt() { return lookAt;}
	Matrix getViewMatrix();

	void update();
};

#endif // ! _CAMERA_H_
