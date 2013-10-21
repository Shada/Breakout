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
	BBUI(Vec2 p, Vec2 s, float r, Vec4 ta, int i) : pos(p), size(s), rotation(r), tintAlpha(ta), texIndex(i){}
	BBUI(){}
};
struct BBFont
{
	float x; //the x position of the letter
	Vec4 texCoords; //x,y,width,height on image only. (For world position, the entire text will use CBFont)
};
struct CBFont
{
	Vec4 tintAlpha;			//rgb tint, a alpha
	Vec2 pos;				//world position
	Vec2 scale;				//world scale
	Vec2 imageSize;			//resolution of font image
	float rotation;			//world rotation
	float padda;
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
	Matrix viewRefl;
	Vec3 cameraPos;
	float p1;
	Vec3 cameraDir;
	float p2;
};
struct CBOnce
{
	Matrix	projection;
	Matrix	projectionInv;
	Vec4	lightPos;
	Vec2	resolution;
	Vec2	padding2;
};
struct CBWater
{
	float waterLevel;
	float timer;
	Vec2 windDirection;
};
struct CBWaterOnce
{
	float waterFade; //how deep anything need to be to fade out in the water
	float normalScaling; 
	float maxAmplitude; //max wave amplitude
	float shoreTransition; //how soft the water and ground fades

	float refractionStrength; //refraction strength
	float displacementStrength; //multiplier for the height of waves
	float shininess;
	float specularIntensity;

	float transparency;
	float refractionScale;
	Vec2 scale;

	Vec4 normalModifier; //multiplier for the different normals. first one is for small waves.

	Vec4 foamOptions; //depth of which foam starts to fade out, depth of which foam is invisible, height of which foam appears for waves.

	Vec3 waterSurfaceColor;
	int  waterType;

	Vec4 waterDepthColor;

	Vec4 extinction;

};

struct Action2D
{
	Vec2	newPos;			//new position value
	Vec2	newScale;		//new scale value
	float	newRotation;	//new rotation value
	Vec4	newTintAlpha;	//new tint alpha values
	float	time;			//interpolation time
	float	timePassed;		//time passed since start. (if negative, it is inactive until positive)
	int		expFactor;		//what order of exponential interpolation?

	Action2D(Vec2 inPos, Vec2 inScale, float inRotation, Vec4 inTintAlpha, float inTime, int inExpFactor)
		: newPos(inPos), newScale(inScale), newRotation(inRotation),newTintAlpha(inTintAlpha), time(inTime), expFactor(inExpFactor){ timePassed = 0; }
    Action2D(){}
};

struct MinorEffect 
{
	Vec3 pos;
	int type;
};


#define PI 3.14159265359
#define SCRWIDTH 1920
#define SCRHEIGHT 1080
#define FULLSCR 0
#define SAFE_RELEASE(x) if(x) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x)	if(x){ delete(x);		(x) = NULL; }

enum KeyCodes
{
	KC_A = 0x41,		KC_C = 0x43,		KC_B = 0x42,		KC_D = 0x44,		
	KC_E = 0x45,		KC_F = 0x46,		KC_G = 0x47,		KC_H = 0x48,		
	KC_I = 0x49,		KC_J = 0x4A,		KC_K = 0x4B,		KC_L = 0x4C,				
	KC_M = 0x4D,		KC_N = 0x4E,		KC_O = 0x4F,		KC_P = 0x50,		
	KC_Q = 0x51,		KC_R = 0x52,		KC_S = 0x53,		KC_T = 0x54,		
	KC_U = 0x55,		KC_V = 0x56,		KC_W = 0x57,		KC_X = 0x58,
	KC_Y = 0x59,		KC_Z = 0x5A,		KC_0 = 0x30,		KC_1 = 0x31,
	KC_2 = 0x32,		KC_3 = 0x33,		KC_4 = 0x34,		KC_5 = 0x35,
	KC_6 = 0x36,		KC_7 = 0x37,		KC_8 = 0x38,		KC_9 = 0x39,
	KC_SPACE = 0x20,

#ifdef _WIN32
	KC_PAGEUP		= 0x21,
	KC_PAGEDOWN		= 0x22,
	KC_END			= 0x23,
	KC_HOME			= 0x24,
	KC_LEFT			= 0x25,
	KC_UP			= 0x26,
	KC_RIGHT		= 0x27,
	KC_DOWN			= 0x28,
	KC_INSERT		= 0x2D,
	KC_DELETE		= 0x2E,
	KC_NUMPAD0		= 0x60,
	KC_NUMPAD1		= 0x61,
	KC_NUMPAD2		= 0x62,
	KC_NUMPAD3		= 0x63,
	KC_NUMPAD4		= 0x64,
	KC_NUMPAD5		= 0x65,
	KC_NUMPAD6		= 0x66,
	KC_NUMPAD7		= 0x67,
	KC_NUMPAD8		= 0x68,
	KC_NUMPAD9		= 0x69,
#else
	KC_INSERT		= 0x128,
	KC_DELETE		= 0x129,
	KC_RIGHT		= 0x11E,
	KC_LEFT			= 0x11D,
	KC_DOWN			= 0x11C,
	KC_UP			= 0x11B,
	KC_PAGEUP		= 0x12A,
	KC_PAGEDOWN		= 0x12B,
	KC_HOME			= 0x12C,
	KC_END			= 0x12D,
	KC_NUMPAD0		= 0x12E,
	KC_NUMPAD1		= 0x12F,
	KC_NUMPAD2		= 0x130,
	KC_NUMPAD3		= 0x131,
	KC_NUMPAD4		= 0x132,
	KC_NUMPAD5		= 0x133,
	KC_NUMPAD6		= 0x134,
	KC_NUMPAD7		= 0x135,
	KC_NUMPAD8		= 0x136,
	KC_NUMPAD9		= 0x137,
#endif
};

enum GameState
{
	GAME_MENU = 0,
	GAME_PLAY = 1,
	GAME_PLAYING = 2,
};

//the game state (found in Resource.h)
//static GameState gameState = GameState::GAME_PLAY;

#endif // ! _RESOURCE_H_