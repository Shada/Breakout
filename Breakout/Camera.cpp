#include "Camera.h"
#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#endif // _WIN32
Camera::Camera()
{
	position = Vec3(100, 75, -150);
	//position = Vec3(512, 384, -512);
	rotation = Vec3(0, 0, 0);

#ifndef _WIN32
    // Send pointers to camera matrices to graphic engine
    GraphicsOGL4::getInstance()->updateProjectionMatrix(&projectionMatrix);
    GraphicsOGL4::getInstance()->updateViewMatrix(&viewMatrix);
    GraphicsOGL4::getInstance()->updateViewInverseMatrix(&viewInv);
    GraphicsOGL4::getInstance()->updateProjectionInverseMatrix(&projectionInv);
#endif

#ifdef _WIN32
	CBOnce cbonce;
	perspectiveFovLH(projectionMatrix, PI * 0.5, (float)SCRWIDTH / SCRHEIGHT, 0.01, 600);
#else
    perspectiveFovRH(projectionMatrix, PI * 0.5, float(SCRWIDTH/SCRHEIGHT), 0.1f, 600.f);
#endif //_WIN32
	MatrixInversion(projectionInv, projectionMatrix);

#ifdef _WIN32
	cbonce.projection = projectionMatrix;
	cbonce.projectionInv = projectionInv;
	cbonce.lightPos = Vec4(500, 1000, -500, 1);
	GraphicsDX11::getInstance()->updateCBOnce(cbonce);
#endif //_ WIN32
}

Camera::~Camera()
{
}

void Camera::setPosition(Vec3 _position)
{
	position = _position;
}

void Camera::setRotation(Vec3 _rotation)
{
	rotation = _rotation;
}

Vec3 Camera::getPosition()
{
	return position;
}

Vec3 Camera::getRotation()
{
	return rotation;
}

void Camera::update()
{
	Vec3 up, pos, lookAt, rot;
	Matrix rotationMatrix;
	float radianConv = (float)(PI/180); //Used to convert from degree to radians

	//Setup up-, pos- and look-vectors
	up = Vec3(0,1,0);
	pos = position;
	lookAt = Vec3(0,0,1);

	//Set yaw, pitch and roll rotations in radians
	rot.x = rotation.x * radianConv;
	rot.y = rotation.y * radianConv;
	rot.z = rotation.z * radianConv;

	//Create rotation matrix
	rotationMatrix = yawPitchRoll(rot);

	//Transform lookAt and up vector by rotation matrix
	transformCoord(lookAt, lookAt, rotationMatrix);
	transformCoord(up, up, rotationMatrix);


	//Translate rotated camera position to location of viewer
	//lookAt = pos + lookAt;

	//Create view matrix from vectors
#ifdef _WIN32
	lookAtLH(viewMatrix, lookAt, up, pos); //Pos might not be correct, needs testing.
#else
    lookAtRH(viewMatrix, lookAt, up, pos);
#endif // _WIN32

	MatrixInversion(viewInv, viewMatrix);

#ifdef _WIN32
	CBCameraMove cb;

	cb.cameraPos = pos;
	cb.cameraDir = lookAt - pos;

	cb.View = viewMatrix;
	cb.ViewInv = viewInv;

	GraphicsDX11::getInstance()->updateCBCameraMove(cb);
#endif
}

Matrix Camera::getViewMatrix()
{
	return viewMatrix;
}
