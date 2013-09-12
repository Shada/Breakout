#pragma once

// If compiled on win32 system, _WIN32 specific 
#ifdef _WIN32
	#include <windows.h>
	#include "GraphicsDX11.h"
#else
	#include "GraphicsOGL4.h"
#endif

class Windowhandler
{
protected:

	virtual bool initWindow() = 0;
	virtual void createConsoleLog(const char *winTitle) = 0;
public:
	virtual int run() = 0;
	Windowhandler();

	// virtual destructor
	virtual ~Windowhandler();
};
#ifdef _WIN32
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

#else
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
#endif
