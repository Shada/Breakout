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
public:
	virtual int run() = 0;
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
	HRESULT initWindow();
	HRESULT initWindow(HINSTANCE hInstance, int nCmdShow);
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
	//GLFWwindow *hwnd;
	bool initWindow();
	void createConsoleLog(const char *winTitle);
public:
	int run();
	Linuxhandler();
	~Linuxhandler();
};
#endif
