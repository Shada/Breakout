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

	void SetPosition(Vec3 _position);
	void SetRotation(Vec3 rotation);

	Vec3 GetPosition();
	Vec3 GetRotation();
	Matrix GetViewMatrix();

	void Update();
};

#endif