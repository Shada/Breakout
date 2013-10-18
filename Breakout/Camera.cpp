#include "Camera.h"
#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#endif // _WIN32
Camera::Camera()
{

	position = Vec3(150, 100, -112);
	rotation = Vec3(0, 0, 0);
	//lookAt = Vec3(75,75,1);
	lookAt = Vec3 (150, 100 ,-50);

#ifndef _WIN32
	//TODO: REMOVE!!!

    // Send pointers to camera matrices to graphic engine
    GraphicsOGL4::getInstance()->updateProjectionMatrix(&projectionMatrix);
    GraphicsOGL4::getInstance()->updateViewMatrix(&viewMatrix);
    GraphicsOGL4::getInstance()->updateViewInverseMatrix(&viewInv);
    GraphicsOGL4::getInstance()->updateProjectionInverseMatrix(&projectionInv);
#endif

    perspectiveFovLH(projectionMatrix, (float)PI * 0.5, (float)SCRWIDTH / SCRHEIGHT, 0.01f, 600.f);
	
	MatrixInversion(projectionInv, projectionMatrix);


	CBOnce cbonce;
	cbonce.projection = projectionMatrix;
	cbonce.projectionInv = projectionInv;
	cbonce.lightPos = Vec4(500, 1000, -500, 1);
	cbonce.resolution = Vec2(SCRWIDTH, SCRHEIGHT);
#ifdef _WIN32	
	GraphicsDX11::getInstance()->updateCBOnce(cbonce);
#else
	GraphicsOGL4::getInstance()->updateCBOnce(cbonce);
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
	Vec3 up, pos, rot, negUp;
	Matrix rotationMatrix;
	float radianConv = (float)(PI)/180; //Used to convert from degree to radians

	//Setup up-, pos- and look-vectors
	up = Vec3(0,1,0);
	negUp = Vec3(0,-1,0);
	pos = position;

	//Set yaw, pitch and roll rotations in radians
	rot.x = rotation.x * radianConv;
	rot.y = rotation.y * radianConv;
	rot.z = rotation.z * radianConv;

	//Create rotation matrix
	rotationMatrix = yawPitchRoll(rot);

	//Transform lookAt and up vector by rotation matrix
	transformCoord(lookAt, lookAt, rotationMatrix);
	transformCoord(up, up, rotationMatrix);
	transformCoord(negUp, negUp, rotationMatrix);

	//Translate rotated camera position to location of viewer
	//lookAt = pos + Vec3(0,0,1);

	//Create view matrix from vectors

	lookAtLHP(viewMatrix, lookAt, up, pos); //Pos might not be correct, needs testing.
	Vec3 reflPos = pos;
	reflPos.y = waterLevel - (reflPos.y - waterLevel);
	lookAtLHP(viewRefl,lookAt,negUp,reflPos);

	MatrixInversion(viewInv, viewMatrix);

	CBCameraMove cb;

	cb.cameraPos = pos;
	cb.cameraDir = lookAt - pos;
	cb.viewRefl = viewRefl;
	cb.View = viewMatrix;
	cb.ViewInv = viewInv;
#ifdef _WIN32
	GraphicsDX11::getInstance()->updateCBCameraMove(cb);
#else
	GraphicsOGL4::getInstance()->updateCBCameraMove(cb);
#endif
}

Matrix Camera::getViewMatrix()
{
	return viewMatrix;
}
