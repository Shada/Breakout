#ifndef _INPUTHANDLER_H_
#define _INPUTHANDLER_H_

#include <vector>
#include <functional>
#include "Pad.h"
#include "Menu.h"

struct KeyBind
{
	int keyCode;
	std::function<void()> func;
	KeyBind(int _key, std::function<void()> _func) : keyCode(_key), func(_func) {}
};
struct KeyBind2
{
	int keyCode;
	std::function<void(Logic::Menu*)> func;
	KeyBind2(int _key, std::function<void(Logic::Menu*)> _func) : keyCode(_key), func(_func) {}
};

class Inputhandler
{
protected:
	struct MenuControls
	{
		Logic::Menu *menu;
		std::vector<KeyBind2> keyBindings;
	};
	struct PadControls
	{
		Logic::Pad *pad;
		std::vector<KeyBind> keyBindings;
	};

	MenuControls men;
	PadControls pad;
	std::vector<bool>		prevKeyPressed;

public:
	Inputhandler();
	virtual ~Inputhandler();

	virtual void updateMenu() = 0;
	virtual void updateGame() = 0;
	virtual void setPad(Logic::Pad *_pad, std::vector<KeyBind> _keys);
	virtual void setMenu(Logic::Menu *men, std::vector<KeyBind2> keys);
};

#ifdef _WIN32
#include <dinput.h>
class DInputhandler : public Inputhandler
{
private:

	POINT setMouse;
	HWND					hWnd;

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
	int mouseX, mouseY, prevMouseZ;
	void update();
public:
	GLInputhandler();
	~GLInputhandler();

	void updateMenu();
	void updateGame();
};

#endif // _WIN32
#endif // ! _INPUTHANDLER_H_
