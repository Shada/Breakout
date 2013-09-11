#pragma once
#include <windows.h>

class Windowhandler
{
protected:

	virtual HRESULT initWindow() = 0;

	virtual void createConsoleLog(const char *winTitle) = 0;
	virtual int run() = 0;
public:
	Windowhandler();
	~Windowhandler();
};

class Winhandler : public Windowhandler
{
private:
	HWND hWnd;
	HINSTANCE hInst;

	void createConsoleLog(const char *winTitle);
	int run();
	HRESULT initWindow();
	HRESULT initWindow(HINSTANCE hInstance, int nCmdShow);
public:
	Winhandler();
	~Winhandler();
};

class Linuxhandler : public Windowhandler
{
//#include <gl/glew.h>
private:
	HRESULT initWindow();
	void createConsoleLog(const char *winTitle);
	int run();
public:
	Linuxhandler();
	~Linuxhandler();
};