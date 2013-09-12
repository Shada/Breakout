#include "Windowhandler.h"
#include <io.h>
#include <iostream>
#include <fcntl.h>

#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#endif



Windowhandler::Windowhandler()
{
	
}

Windowhandler::~Windowhandler()
{
}


Winhandler::Winhandler() : Windowhandler()
{
	createConsoleLog("Output Console");
	
	if(FAILED(initWindow()))
	{
		MessageBox(0, "Error initializing window!", 0, 0);
		return;
	}

	run();
}

HRESULT Winhandler::initWindow()
{
	return initWindow(GetModuleHandle(NULL), SW_SHOW);
	
}

LRESULT CALLBACK wndProc(HWND _hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_PAINT:
		hdc = BeginPaint(_hWnd, &ps);
		EndPaint(_hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(_hWnd, message, wParam, lParam);
	}

	return 0;
}

HRESULT Winhandler::initWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = (WNDPROC)wndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = 0;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = "Huvudkrav";
	wcex.hIconSm        = 0;
	if( !RegisterClassEx(&wcex) )
		return E_FAIL;

	// Create window
	hInst				= hInstance; 
	RECT rc = { 0, 0, 1100, 700 };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
	
	if(!(hWnd = CreateWindow(	"Huvudkrav",
							"Subterranean City of Huvudkrav",
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							rc.right - rc.left,
							rc.bottom - rc.top,
							NULL,
							NULL,
							hInstance,
							NULL)))
	{
		return E_FAIL;
	}

	ShowWindow( hWnd, nCmdShow );
	
	return S_OK;
}

int Winhandler::run()
{
	__int64			currTimeStamp = 0, prevTimeStamp = 0, cntsPerSec = 0;
	QueryPerformanceFrequency( ( LARGE_INTEGER* )&cntsPerSec);
	double			dt = 0, time = 0;
	double			secsPerCnt = 1.0 / (double)cntsPerSec;

	QueryPerformanceCounter( ( LARGE_INTEGER* )&currTimeStamp);
	prevTimeStamp	= currTimeStamp;
	MSG msg			= { 0 };
	while( WM_QUIT != msg.message )
	{
		prevTimeStamp = currTimeStamp;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
		dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;
		time += dt;

		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else if(time > 1.f / 60)
		{
			if(GetActiveWindow() == hWnd)
			{

			}

			time = 0;
		}
	}

	return (int) msg.wParam;
}

void Winhandler::createConsoleLog(const char *winTitle)
{
	AllocConsole();
	SetConsoleTitle(winTitle);

	int hConHandle;
	long lStdHandle;
	FILE fp;

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = *_fdopen( hConHandle, "w" );
	*stdout = fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = *_fdopen( hConHandle, "r" );
	*stdin = fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = *_fdopen( hConHandle, "w" );
	*stderr = fp;
	setvbuf( stderr, NULL, _IONBF, 0 );
}

Winhandler::~Winhandler()
{

}

Linuxhandler::Linuxhandler() : Windowhandler()
{
}

int Linuxhandler::run()
{
	return 0;
}

HRESULT Linuxhandler::initWindow()
{
	return S_OK;
}

void Linuxhandler::createConsoleLog(const char *winTitle)
{
}

Linuxhandler::~Linuxhandler()
{
}