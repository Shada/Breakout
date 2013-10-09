#ifndef _WINDOWHANDLER_H_
#define _WINDOWHANDLER_H_


// If compiled on win32 system, BAJSAPA specific 
#ifdef BAJSAPA
	#include <windows.h>
	#include "GraphicsDX11.h"
#else // BAJSAPA
	#include "GraphicsOGL4.h"
#endif // BAJSAPA
	
#include "Inputhandler.h"
#include "GameLogic.h"
#include "Timer.h"
	
class Windowhandler
{
protected:
	Inputhandler *input;

	virtual bool initWindow() = 0;
	virtual void createConsoleLog(const char *winTitle) = 0;
	Logic::GameLogic *game;
	Timer *timer;
public:
	virtual int run() = 0;
	Windowhandler();

	// virtual destructor
	virtual ~Windowhandler();
};

#ifdef BAJSAPA
class Winhandler : public Windowhandler
{
private:
	HWND hWnd;
	HINSTANCE hInst;

	GraphicsDX11 *g;

	void createConsoleLog(const char *winTitle);
	bool initWindow();
	bool initWindow(HINSTANCE hInstance, int nCmdShow);
public:
	int run();
	Winhandler();
	~Winhandler();
};

#else // BAJSAPA
class Linuxhandler : public Windowhandler
{
private:

	bool initWindow();
	void createConsoleLog(const char *winTitle);
public:
	int run();
	Linuxhandler();
	~Linuxhandler();
};
#endif // BAJSAPA

#endif // !_WINDOWHANDLER_H_