#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "linearalgebra.h"

class Camera
{

private:
	Vec3 position, rotation;
	Matrix viewMatrix;

public:
	Camera();
	~Camera();

	void setPosition(Vec3 position);
	void setRotation(Vec3 rotation);

	Vec3 getPosition();
	Vec3 getRotation();
	Matrix getViewMatrix();

	void update();
};

#endif