#include "Camera.h"

Camera::Camera()
{
	position = Vec3(0,0,0);
	rotation = Vec3(0,0,0);
}


Camera::~Camera()
{
}

void Camera::SetPosition(Vec3 _position)
{
	position = _position;
}

void Camera::SetRotation(Vec3 _rotation)
{
	rotation = _rotation;
}

Vec3 Camera::GetPosition()
{
	return position;
}

Vec3 Camera::GetRotation()
{
	return rotation;
}

void Camera::Update()
{
	Vec3 up, pos, lookAt, rot;
	Matrix rotationMatrix;
	float pi = 3.1415926f;
	float radianConv = pi/180; //Used to convert from degree to radians

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
	lookAt = pos + lookAt;

	//Create view matrix from vectors
	lookAtLH(viewMatrix, lookAt, up, pos); //Pos might not be correct, needs testing.
}

Matrix Camera::GetViewMatrix()
{
	return viewMatrix;
}