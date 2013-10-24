#include "Inputhandler.h"

Inputhandler::Inputhandler()
{
}

void Inputhandler::setPad(Logic::Pad *_pad, std::vector<KeyBind> _keys)
{
	pad.pad = _pad;
	pad.keyBindings = _keys;

	//if size of listenerkeys != functions -> do something (like resize the big one to fit the small one and then warn the user)
}

void Inputhandler::setMenu(Logic::Menu *_men, std::vector<KeyBind2> _keys)
{
	men.menu = _men;
	men.keyBindings = _keys;

	//if size of listenerkeys != functions -> do something (like resize the big one to fit the small one and then warn the user)
}

Inputhandler::~Inputhandler()
{
}

#ifdef _WIN32
DInputhandler::DInputhandler(HWND *hWnd)
{
	directInput			= 0;
	keyboardInput		= 0;
	mouseInput			= 0;

	if(FAILED(DirectInput8Create(GetModuleHandle(NULL),
								DIRECTINPUT_VERSION,
								IID_IDirectInput8,
								(void**)&directInput,
								NULL)))
		return;

	//initKeyboard(hWnd);

	RECT lp;
	GetWindowRect(*hWnd, &lp);
	setMouse.x = lp.left + 400;
	setMouse.y = lp.top + 400;
	ShowCursor(false);
	SetCursorPos(setMouse.x, setMouse.y);
	initMouse(hWnd);
	initKeyboard(hWnd);

	this->hWnd = *hWnd;

	if(FAILED(mouseInput->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState)))
		return;
	prevMouseState = mouseState;
}

HRESULT DInputhandler::initKeyboard(HWND* hWnd)
{
	HRESULT result;
	if(FAILED(result = directInput->CreateDevice(GUID_SysKeyboard,
									&keyboardInput,
									NULL)))
		return result;

	if(FAILED(result = keyboardInput->SetDataFormat(&c_dfDIKeyboard)))
		return result;

	if(FAILED(result = keyboardInput->SetCooperativeLevel(*hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		return result;

	if(FAILED(keyboardInput->Acquire()))
		return result;

	return result;
}

HRESULT DInputhandler::initMouse(HWND* hWnd)
{
	HRESULT result;
	if(FAILED(result = directInput->CreateDevice(GUID_SysMouse,
								&mouseInput,
								NULL)))
		return result;

	if(FAILED(result = mouseInput->SetDataFormat(&c_dfDIMouse)))
		return result;

	if(FAILED(result = mouseInput->SetCooperativeLevel(*hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return result;

	if(FAILED(mouseInput->Acquire()))
		return result;

	return result;
}

void DInputhandler::setMenu(Logic::Menu *_men, std::vector<KeyBind2> _keys)
{
	men.menu = _men;
	men.keyBindings = _keys;

	prevKeyPressed = std::vector<bool>(_keys.size(), false);
}

void DInputhandler::update()
{
	for(int i = 0; i < 256; i++)
		prevKeyState[i] = keyState[i];

	prevMouseState = mouseState;

	HRESULT result = mouseInput->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if(FAILED(result))
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			mouseInput->Acquire();

	result = keyboardInput->GetDeviceState(256, (LPVOID)&keyState);
	if(FAILED(result))
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			keyboardInput->Acquire();
}

void DInputhandler::updateGame()
{
	update();

	for(unsigned int i = 0; i < pad.keyBindings.size(); i++)
		if(GetAsyncKeyState(pad.keyBindings.at(i).keyCode) != 0)// keyState[pad.keyBindings.at(i).keyCode] & 0x80)
			pad.keyBindings.at(i).func();

	if(mouseState.rgbButtons[0] && !prevMouseState.rgbButtons[0])
		pad.pad->ejectBall();

	// These cannot be changed, the mouse will always be one way of controlling the pad
	if(mouseState.lX != 0)
		pad.pad->move(mouseState.lX / 4);

	if(mouseState.lZ != 0)
		pad.pad->rotate(mouseState.lZ < 0 ? -1 : 1);

	RECT r;
	GetWindowRect(hWnd, &r);
	setMouse.x = r.left + 400;
	setMouse.y = r.top + 400;
	SetCursorPos(setMouse.x, setMouse.y);

	pad.pad->updateWorld();
}

void DInputhandler::updateMenu()
{
	update();

	for(unsigned int i = 0; i < men.keyBindings.size(); i++)
	{
		if(GetAsyncKeyState(men.keyBindings.at(i).keyCode) != 0 && !prevKeyPressed.at(i))
		{
			men.keyBindings.at(i).func(men.menu);
			prevKeyPressed.at(i) = true;
		}
		else if(GetAsyncKeyState(men.keyBindings.at(i).keyCode) == 0 && prevKeyPressed.at(i))
			prevKeyPressed.at(i) = false;
	}
}

DInputhandler::~DInputhandler()
{
}

#else
GLInputhandler::GLInputhandler()
{
}

void GLInputhandler::updateGame()
{
	int prevMouseX = mouseX, mouseZ;
	glfwGetMousePos(&mouseX, &mouseY);
	mouseZ = glfwGetMouseWheel();

	for(unsigned int i = 0; i < pad.keyBindings.size(); i++)
		if(glfwGetKey(pad.keyBindings.at(i).keyCode) == GLFW_PRESS)
			pad.keyBindings.at(i).func();

	// These cannot be changed, the mouse will always be one way of controlling the pad
	if(glfwGetKey(GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		pad.pad->ejectBall();

	if(mouseX != prevMouseX)
		pad.pad->move(mouseX - prevMouseX);

	if(mouseZ != prevMouseZ)
		pad.pad->rotate(mouseZ - prevMouseZ > 0 ? -1 : 1);

	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		pad.pad->ejectBall();

	glfwSetMousePos(400, 400);
	glfwDisable(GLFW_MOUSE_CURSOR);
	pad.pad->updateWorld();
	prevMouseZ = mouseZ;
	mouseX = 400;
}

void GLInputhandler::updateMenu()
{
	int prevMouseX = mouseX;
	glfwGetMousePos(&mouseX, &mouseY);

	for(unsigned int i = 0; i < cam.keyBindings.size(); i++)
		if(glfwGetKey(cam.keyBindings.at(i).keyCode) == GLFW_PRESS)
			cam.keyBindings.at(i).func(mouseX, mouseY);
}

GLInputhandler::~GLInputhandler()
{
}
#endif
