#ifndef _INPUTHANDLER_H_
#define _INPUTHANDLER_H_

#include <vector>
#include <functional>
#include "Pad.h"
//#include "Camera.h"

class Inputhandler
{
protected:
	struct CameraControls
	{
		//Camera *camera;
		std::vector<int> listenerKeys;
		std::vector<std::function<void(int, int)>> functions;
		std::function<void(int)> mouseMove;
	};
	struct PadControls
	{
		Logic::Pad *pad;
		std::vector<int> listenerKeys;
		std::vector<std::function<void()>> functions;
	};

	CameraControls cam;
	PadControls pad;

public:
	Inputhandler();
	~Inputhandler();

	virtual void update() = 0;
	virtual void setPad(Logic::Pad *pad, std::vector<int> keys, std::vector<std::function<void()>> functioncalls);
	virtual void setCamera(/*Camera *cam,*/ std::vector<int> keys, std::vector<std::function<void(int, int)>> functioncalls, std::function<void(int)> mouseMove);
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

public:
	DInputhandler(HWND *hWnd);
	~DInputhandler();

	void update();
	//void setPad(Logic::Pad *pad, std::vector<int> keys, std::vector<std::function<void()>> functioncalls);
	//void setCamera(/*Camera *cam,*/ std::vector<int> keys);
};

//#else

#define GLFW_INCLUDE_GLU
#include <GL/glfw.h>

class GLInputhandler : public Inputhandler
{
private:
	int mouseX, mouseY;
public:
	GLInputhandler();
	~GLInputhandler();

	void update();
	//void setPad(Logic::Pad *pad, std::vector<int> keys, std::vector<std::function<void()>> functioncalls);
	//void setCamera(/*Camera *cam,*/ std::vector<int> keys);
};

#endif
#endif