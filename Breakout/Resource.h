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
	int texIndex;
};
struct BBFont
{
	float x; //the x position of the letter
	Vec4 texCoords; //x,y,width,height on image only. (For world position, the entire text will use CBFont)
};
struct CBFont
{
	Vec2 pos;
	Vec2 scale;
	float rotation;
	Vec4 tintAlpha;
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
	Vec2	resolution;
	Vec2	padding2;
};

#define PI 3.14159265359
#define SCRWIDTH 1024
#define SCRHEIGHT 768
#define SAFE_RELEASE(x) if(x) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x)	if(x){ delete(x);		(x) = NULL; }

enum KeyCodes
{
	KC_A = 0x41,		KC_0		= 0x30,		KC_SPACE		= 0x20,
	KC_C = 0x43,		KC_1		= 0x31,		KC_PAGEUP		= 0x21,
	KC_B = 0x42,		KC_2		= 0x32,		KC_PAGEDOWN		= 0x22,
	KC_D = 0x44,		KC_3		= 0x33,		KC_END			= 0x23,
	KC_E = 0x45,		KC_4		= 0x34,		KC_HOME			= 0x24,
	KC_F = 0x46,		KC_5		= 0x35,		KC_LEFT			= 0x25,
	KC_G = 0x47,		KC_6		= 0x36,		KC_UP			= 0x26,
	KC_H = 0x48,		KC_7		= 0x37,		KC_RIGHT		= 0x27,
	KC_I = 0x49,		KC_8		= 0x38,		KC_DOWN			= 0x28,
	KC_J = 0x4A,		KC_9		= 0x39,		KC_PRINTSCRN	= 0x2C,
	KC_K = 0x4B,		KC_NUMPAD0	= 0x60,		KC_INSERT		= 0x2D,
	KC_L = 0x4C,		KC_NUMPAD1	= 0x61,		KC_DELETE		= 0x2E,
	KC_M = 0x4D,		KC_NUMPAD2	= 0x62,
	KC_N = 0x4E,		KC_NUMPAD3	= 0x63,
	KC_O = 0x4F,		KC_NUMPAD4	= 0x64,
	KC_P = 0x50,		KC_NUMPAD5	= 0x65,
	KC_Q = 0x51,		KC_NUMPAD6	= 0x66,
	KC_R = 0x52,		KC_NUMPAD7	= 0x67,
	KC_S = 0x53,		KC_NUMPAD8	= 0x68,
	KC_T = 0x54,		KC_NUMPAD9	= 0x69,
	KC_U = 0x55,		
	KC_V = 0x56,		
	KC_W = 0x57,		
	KC_X = 0x58,
	KC_Y = 0x59,
	KC_Z = 0x5A,
};

enum GameState
{
	GAME_MENU = 0,
	GAME_PLAY = 1,
	GAME_PLAYING = 2
};

//the game state (found in Resource.h)
//static GameState gameState = GameState::GAME_PLAY;

#endif // ! _RESOURCE_H_