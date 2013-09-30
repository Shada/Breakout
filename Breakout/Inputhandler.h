#ifndef _INPUTHANDLER_H_
#define _INPUTHANDLER_H_

#include <vector>
#include <functional>
#include "Pad.h"
#include "Camera.h"

struct KeyBind
{
	int keyCode;
	std::function<void()> func;
	KeyBind(int _key, std::function<void()> _func) : keyCode(_key), func(_func) {}
};
struct KeyBind2
{
	int keyCode;
	std::function<void(int, int)> func;
	KeyBind2(int _key, std::function<void(int, int)> _func) : keyCode(_key), func(_func) {}
};

class Inputhandler
{
protected:
	struct MenuControls
	{
		Camera *cam;
		std::vector<KeyBind2> keyBindings;
	};
	struct PadControls
	{
		Logic::Pad *pad;
		std::vector<KeyBind> keyBindings;
	};

	MenuControls cam;
	PadControls pad;

public:
	Inputhandler();
	virtual ~Inputhandler();

	virtual void updateMenu() = 0;
	virtual void updateGame() = 0;
	virtual void setPad(Logic::Pad *_pad, std::vector<KeyBind> _keys);
	virtual void setCamera(Camera *cam, std::vector<KeyBind2> keys);
};

#ifdef BAJSAPA
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
};

#else

#include <GL/glew.h>
#include <GL/glfw.h>

class GLInputhandler : public Inputhandler
{
private:
	int mouseX, mouseY;
	void update();
public:
	GLInputhandler();
	~GLInputhandler();

	void updateMenu();
	void updateGame();
};

#endif // BAJSAPA
#endif // ! _INPUTHANDLER_H_
