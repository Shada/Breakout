#include "Inputhandler.h"

Inputhandler::Inputhandler()
{
}

void Inputhandler::setPad(Logic::Pad *_pad, std::vector<int> _keys, std::vector<std::function<void(int)>> _functioncalls, std::vector<int> _directions)
{
	pad.pad = _pad;
	pad.listenerKeys = _keys;
	pad.functions = _functioncalls;
	pad.directions = _directions;

	//if size of listenerkeys != functions -> do something (like resize the big one to fit the small one and then warn the user)
}

void Inputhandler::setCamera(Camera *_cam, std::vector<int> _keys, std::vector<std::function<void(int, int)>> _functioncalls)
{
	cam.cam = _cam;
	cam.listenerKeys = _keys;
	cam.functions = _functioncalls;

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

	initKeyboard(hWnd);
	initMouse(hWnd);

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

	for(unsigned int i = 0; i < pad.listenerKeys.size(); i++)
		if(keyState[pad.listenerKeys.at(i)] & 0x80)
			pad.functions.at(i)(pad.directions.at(i));

	// These cannot be changed, the mouse will always be one way of controlling the pad
	if(mouseState.lX != 0)
		pad.pad->move(mouseState.lX);

	if(mouseState.lZ != 0)
		pad.pad->rotate(mouseState.lZ < 0 ? -1 : 1);
}

void DInputhandler::updateMenu()
{
	update();

	for(unsigned int i = 0; i < cam.listenerKeys.size(); i++)
		if((keyState[cam.listenerKeys.at(i)] & 0x80) && !(prevKeyState[cam.listenerKeys.at(i)] & 0x80))
			cam.functions.at(i)(mouseState.lX, mouseState.lY);
}

DInputhandler::~DInputhandler()
{
}

#else
GLInputhandler::GLInputhandler()
{
}

void GLInputhandler::update()
{
	int prevMouseX = mouseX;
	glfwGetMousePos(&mouseX, &mouseY);

	for(unsigned int i = 0; i < pad.listenerKeys.size(); i++)
		if(glfwGetKey(pad.listenerKeys.at(i)) == GLFW_PRESS)
			pad.functions.at(i)();

	for(unsigned int i = 0; i < cam.listenerKeys.size(); i++)
		if(glfwGetKey(cam.listenerKeys.at(i)) == GLFW_PRESS)
			cam.functions.at(i)(mouseX, mouseY);

	if(prevMouseX != mouseX)
		pad.mouseMove(mouseX - prevMouseX);
}

void GLInputhandler::updateGame()
{

}

void GLInputhandler::updateMenu()
{

}

GLInputhandler::~GLInputhandler()
{
}
#endif