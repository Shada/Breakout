#include "Camera.h"
#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#endif // _WIN32
Camera::Camera()
{
	position = Vec3(60, 60, -500);
	rotation = Vec3(0, 0, 0);

	Matrix proj, projInv;

#ifdef _WIN32
	CBOnce cbonce;
#endif //_WIN32

	perspectiveLH(proj, PI * 0.3, float(SCRWIDTH / SCRHEIGHT), 0.01, 600);

#ifdef _WIN32
	cbonce.projection = proj;
#else
    GraphicsOGL4::getInstance()->updateProjectionMatrix(&proj);
#endif //_WIN32

	MatrixInversion(projInv, proj);
#ifdef _WIN32
	cbonce.projectionInv = projInv;
	cbonce.lightPos = Vec4(500, 1000, -500, 1);
#else
	GraphicsOGL4::getInstance()->updateProjectionInverseMatrix(&projInv);
#endif //_WIN32

#ifdef _WIN32
	GraphicsDX11::getInstance()->updateCBOnce(cbonce);
#else
	//send to OpenGL GLSL thingy
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
	float radianConv = PI/180; //Used to convert from degree to radians

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
	lookAtLH(viewMatrix, lookAt, up, pos); //Pos might not be correct, needs testing.

	Matrix viewInv;
	MatrixInversion(viewInv, viewMatrix);

#ifdef _WIN32
	CBCameraMove cb;

	cb.cameraPos = pos;
	cb.cameraDir = lookAt - pos;

	cb.View = viewMatrix;
	cb.ViewInv = viewInv;

	GraphicsDX11::getInstance()->updateCBCameraMove(cb);
#else
    GraphicsOGL4::getInstance()->updateViewMatrix(&viewMatrix);
    GraphicsOGL4::getInstance()->updateViewInverseMatrix(&viewInv);
	//Linix send in view shit
#endif
}

Matrix Camera::getViewMatrix()
{
	return viewMatrix;
}
