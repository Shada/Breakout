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

#define PI 3.14159265359
#define SAFE_RELEASE(x) if(x) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x)	if(x){ delete(x);		(x) = NULL; }

// temporary, just fr being able to test linux code on win32 environment.
//#define _WIN32
#endif