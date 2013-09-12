#pragma once

// If compiled on win32 system, windows specific 
#ifdef WINDOWS
	#include <windows.h>
#else
	#include <gl/glew.h>
	#include <gl/glfw.h>
#endif

class Windowhandler
{
protected:

	virtual bool initWindow() = 0;

	virtual void createConsoleLog(const char *winTitle) = 0;
	virtual int run() = 0;
public:
	Windowhandler();
	~Windowhandler();
};
#ifdef WINDOWS
class Winhandler : public Windowhandler
{
private:
	HWND hWnd;
	HINSTANCE hInst;

	void createConsoleLog(const char *winTitle);
	int run();
	bool initWindow();
	bool initWindow(HINSTANCE hInstance, int nCmdShow);
public:
	Winhandler();
	~Winhandler();
};

#else
class Linuxhandler : public Windowhandler
{
private:
	//GLFWwindow *hwnd;
	bool initWindow();
	void createConsoleLog(const char *winTitle);
	int run();
public:
	Linuxhandler();
	~Linuxhandler();
};
#endif