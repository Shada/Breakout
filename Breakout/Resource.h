#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <stdio.h>
#include <stdlib.h>
#include "linearalgebra.h"

struct Vertex
{
    Vec3 pos;
	Vec3 normal;
	Vec2 texCoord;
    Vertex(Vec3 p, Vec3 n, Vec2 t) : pos(p), normal(n), texCoord(t){}
    Vertex(){}
};
struct PerInstance
{
	Vec3	pos;
	Vec3	rot;
	int		typeID;
};
struct BBUI
{
	Vec2 pos;
	Vec2 size;
	float rotation;
	Vec4 tintAlpha;
	Vec2 texCoords;
	int texIndex;
};
struct CBWorld
{
	Matrix world;
	Matrix worldInv;
};
struct CBCameraMove
{
	Matrix View;		
	Matrix ViewInv;		
	Vec3 cameraPos;		
	Vec3 cameraDir;		

	//byte amount must be dividable by 16
	Vec2 padding;
};

struct CBOnce
{
	Matrix	projection;
	Matrix	projectionInv;
	Vec4	lightPos;
};

#define PI 3.14159265359
#define SCRWIDTH 1024
#define SCRHEIGHT 768
#define SAFE_RELEASE(x) if(x) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x)	if(x){ delete(x);		(x) = NULL; }

enum GameState
{
	GAME_MENU = 0,
	GAME_PLAY = 1
};

//the game state (found in Resource.h)
static GameState gameState = GAME_MENU;

#endif // ! _RESOURCE_H_