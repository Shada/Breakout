#ifndef _INPUTHANDLER_H_
#define _INPUTHANDLER_H_

#include <vector>
#include <functional>
#include "Pad.h"
#include "Camera.h"

class Inputhandler
{
protected:
	struct MenuControls
	{
		Camera *cam;
		std::vector<int> listenerKeys;
		std::vector<std::function<void(int, int)>> functions;
	};
	struct PadControls
	{
		Logic::Pad *pad;
		std::vector<int> listenerKeys;
		std::vector<std::function<void(int)>> functions;
		std::vector<int> directions;
	};

	MenuControls cam;
	PadControls pad;

public:
	Inputhandler();
	~Inputhandler();

	virtual void updateMenu() = 0;
	virtual void updateGame() = 0;
	virtual void setPad(Logic::Pad *pad, std::vector<int> keys, std::vector<std::function<void(int)>> functioncalls, std::vector<int> directions);
	virtual void setCamera(Camera *cam, std::vector<int> keys, std::vector<std::function<void(int, int)>> functioncalls);
};

#ifdef _WIN32
#include <dinput.h>
class DInputhandler : public Inputhandler
{
private:

	LPDIRECTINPUT8			directInput;

	LPDIRECTINPUTDEVICE8	keyboardInput;
	LPDIRECTINPUTDEVICE8	mouseInput;

	char					keyState[256], prevKeyState[256];
	DIMOUSESTATE			mouseState, prevMouseState;

	HRESULT initMouse(HWND* hWnd);
	HRESULT initKeyboard(HWND* hWnd);
	void update();

public:
	DInputhandler(HWND *hWnd);
	~DInputhandler();

	void updateMenu();
	void updateGame();
	//void setPad(Logic::Pad *pad, std::vector<int> keys, std::vector<std::function<void()>> functioncalls);
	//void setCamera(/*Camera *cam,*/ std::vector<int> keys);
};

#else

#define GLFW_INCLUDE_GLU
#include <GL/glfw.h>

class GLInputhandler : public Inputhandler
{
private:
	int mouseX, mouseY;
public:
	GLInputhandler();
	~GLInputhandler();

	void updateMenu();
	void updateGame();
	//void setPad(Logic::Pad *pad, std::vector<int> keys, std::vector<std::function<void()>> functioncalls);
	//void setCamera(/*Camera *cam,*/ std::vector<int> keys);
};

#endif
#endif