#include "Skybox.h"


Skybox::Skybox()
{
}


Skybox::~Skybox()
{
}

std::vector<Vertex> Skybox::generateVertices()
{
	std::vector<Vertex> out = std::vector<Vertex>();

	//front of the cube
	out.push_back(Vertex( Vec3(5.0f,-5.0f,-5.0f), Vec3(0.0f,0.0f,-1.0f),Vec2(0.499f,0.332f)));
	out.push_back(Vertex( Vec3(-5.0f,5.0f,-5.0f), Vec3(0.0f,0.0f,-1.0f),Vec2(0.0f,0.0f)));
	out.push_back(Vertex( Vec3(5.0f,5.0f,-5.0f), Vec3(0.0f,0.0f,-1.0f),Vec2(0.499f,0.0f)));

	out.push_back(Vertex( Vec3(5.0f,-5.0f,-5.0f), Vec3(0.0f,0.0f,-1.0f),Vec2(0.499f,0.332f)));
	out.push_back(Vertex( Vec3(-5.0f,-5.0f,-5.0f), Vec3(0.0f,0.0f,-1.0f),Vec2(0.0f,0.332f)));
	out.push_back(Vertex( Vec3(-5.0f,5.0f,-5.0f), Vec3(0.0f,0.0f,-1.0f),Vec2(0.0f,0.0f)));

	//right side of the cube
	out.push_back(Vertex( Vec3(5.0f,-5.0f,5.0f), Vec3(1.0f,0.0f,0.0f),Vec2(1.0f,0.332f)));
	out.push_back(Vertex( Vec3(5.0f,5.0f,-5.0f), Vec3(1.0f,0.0f,0.0f),Vec2(0.501f,0.0f)));
	out.push_back(Vertex( Vec3(5.0f,5.0f,5.0f), Vec3(1.0f,0.0f,0.0f),Vec2(1.0f,0.0f)));
	//här
	out.push_back(Vertex( Vec3(5.0f,-5.0f,5.0f), Vec3(1.0f,0.0f,0.0f),Vec2(1.0f,0.332f)));
	out.push_back(Vertex( Vec3(5.0f,-5.0f,-5.0f), Vec3(1.0f,0.0f,0.0f),Vec2(0.501f,0.332f)));
	out.push_back(Vertex( Vec3(5.0f,5.0f,-5.0f), Vec3(1.0f,0.0f,0.0f),Vec2(0.501f,0.0f)));

	//back of the cube
	out.push_back(Vertex( Vec3(-5.0f,-5.0f,5.0f), Vec3(0.0f,0.0f,1.0f),Vec2(0.499f,0.666f)));
	out.push_back(Vertex( Vec3(5.0f,5.0f,5.0f), Vec3(0.0f,0.0f,1.0f),Vec2(0.0f,0.334f)));
	out.push_back(Vertex( Vec3(-5.0f,5.0f,5.0f), Vec3(0.0f,0.0f,1.0f),Vec2(0.499f,0.334f)));

	out.push_back(Vertex( Vec3(-5.0f,-5.0f,5.0f), Vec3(0.0f,0.0f,1.0f),Vec2(0.499f,0.666f)));
	out.push_back(Vertex( Vec3(5.0f,-5.0f,5.0f), Vec3(0.0f,0.0f,1.0f),Vec2(0.0f,0.666f)));
	out.push_back(Vertex( Vec3(5.0f,5.0f,5.0f), Vec3(0.0f,0.0f,1.0f),Vec2(0.0f,0.334f)));

	//left side of the cube
	out.push_back(Vertex( Vec3(-5.0f,-5.0f,-5.0f), Vec3(-1.0f,0.0f,0.0f),Vec2(1.0f,0.666f)));
	out.push_back(Vertex( Vec3(-5.0f,5.0f,5.0f), Vec3(-1.0f,0.0f,0.0f),Vec2(0.501f,0.334f)));
	out.push_back(Vertex( Vec3(-5.0f,5.0f,-5.0f), Vec3(-1.0f,0.0f,0.0f),Vec2(1.0f,0.334f)));

	out.push_back(Vertex( Vec3(-5.0f,-5.0f,-5.0f), Vec3(-1.0f,0.0f,0.0f),Vec2(1.0f,0.666f)));
	out.push_back(Vertex( Vec3(-5.0f,-5.0f,5.0f), Vec3(-1.0f,0.0f,0.0f),Vec2(0.501f,0.666f)));
	out.push_back(Vertex( Vec3(-5.0f,5.0f,5.0f), Vec3(-1.0f,0.0f,0.0f),Vec2(0.501f,0.334f)));

	//top of the cube
	out.push_back(Vertex( Vec3(-5.0f,5.0f,5.0f), Vec3(0.0f,1.0f,0.0f),Vec2(0.0f,0.668f)));
	out.push_back(Vertex( Vec3(5.0f,5.0f,-5.0f), Vec3(0.0f,1.0f,0.0f),Vec2(0.499f,1.0f)));
	out.push_back(Vertex( Vec3(-5.0f,5.0f,-5.0f), Vec3(0.0f,-.0f,0.0f),Vec2(0.0f,1.0f)));

	out.push_back(Vertex( Vec3(-5.0f,5.0f,5.0f), Vec3(0.0f,1.0f,0.0f),Vec2(0.0f,0.668f)));
	out.push_back(Vertex( Vec3(5.0f,5.0f,5.0f), Vec3(0.0f,1.0f,0.0f),Vec2(0.499f,0.668f)));
	out.push_back(Vertex( Vec3(5.0f,5.0f,-5.0f), Vec3(0.0f,1.0f,0.0f),Vec2(0.499f,1.0f)));

	//bottom of the cube
	out.push_back(Vertex( Vec3(-5.0f,-5.0f,-5.0f), Vec3(0.0f,-1.0f,0.0f),Vec2(0.501f,0.668f)));
	out.push_back(Vertex( Vec3(5.0f,-5.0f,5.0f), Vec3(0.0f,-1.0f,0.0f),Vec2(1.0f,1.0f)));
	out.push_back(Vertex( Vec3(-5.0f,-5.0f,5.0f), Vec3(0.0f,-1.0f,0.0f),Vec2(0.501f,1.0f)));

	out.push_back(Vertex( Vec3(-5.0f,-5.0f,-5.0f), Vec3(0.0f,-1.0f,0.0f),Vec2(0.501f,0.668f)));
	out.push_back(Vertex( Vec3(5.0f,-5.0f,-5.0f), Vec3(0.0f,-1.0f,0.0f),Vec2(1.0f,0.668f)));
	out.push_back(Vertex( Vec3(5.0f,-5.0f,5.0f), Vec3(0.0f,-1.0f,0.0f),Vec2(1.0f,1.0f)));

	return out;
}