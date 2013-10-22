#include "Windowhandler.h"
#include <iostream>
#include <fcntl.h>
#include "Resource.h"

#ifdef _WIN32
#include "GraphicsDX11.h"
#include <io.h>
#else
#include "GraphicsOGL4.h"
#endif


Windowhandler::Windowhandler()
{
	timer = new Timer();
}

Windowhandler::~Windowhandler()
{
	SAFE_DELETE(timer);
	SAFE_DELETE(game);
}

#ifdef _WIN32
Winhandler::Winhandler() : Windowhandler()
{
	createConsoleLog("Output Console");

	if(FAILED(initWindow()))
	{
		MessageBox(0, "Error initializing window!", 0, 0);
		return;
	}

	g = GraphicsDX11::getInstance();
	g->init(&hWnd);

	input = new DInputhandler(&hWnd);
	game = new Logic::GameLogic(input);
}

bool Winhandler::initWindow()
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

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(_hWnd, message, wParam, lParam);
	}

	return 0;
}

bool Winhandler::initWindow(HINSTANCE hInstance, int nCmdShow)
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
		return false;

	// Create window
	hInst				= hInstance;
	RECT rc = { 0, 0, SCRWIDTH, SCRHEIGHT };

	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	if(!(hWnd = CreateWindow(	"Huvudkrav",
							"Break yo face! ",
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
		return false;
	}

	ShowWindow( hWnd, nCmdShow );

	return true;
}

int Winhandler::run()
{
	/*__int64			currTimeStamp = 0, prevTimeStamp = 0, cntsPerSec = 0;
	QueryPerformanceFrequency( ( LARGE_INTEGER* )&cntsPerSec);
	double			dt = 0, time = 0;
	double			secsPerCnt = 1.0 / (double)cntsPerSec;*/
	float time = 0, fpsUpdate = 0;
	int fps = 0;

	//QueryPerformanceCounter( ( LARGE_INTEGER* )&currTimeStamp);
	//prevTimeStamp	= currTimeStamp;
	MSG msg			= { 0 };

	timer->Tick();

	while( WM_QUIT != msg.message )
	{
		/*prevTimeStamp = currTimeStamp;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
		dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;*/
		timer->Tick();
		time += (float)timer->getDelta();

		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		else if(time > 0.01f)
		{
			fpsUpdate += time;
			fps++;

			if(GetActiveWindow() == hWnd)
			{
				game->update(time);
				if(fpsUpdate >= 1)
				{
					game->setFpsCounter(fps);
					fpsUpdate = 0;
					fps = 0;
				}
			}

			g->clearRenderTarget(0.1f,0.05f,0.5f);

			g->draw();

			g->presentSwapChain();

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
	SAFE_DELETE(g);
}

#else
Linuxhandler::Linuxhandler() : Windowhandler()
{
	//hwnd = NULL;
	//create console? or is it automatic...?
	initWindow();
	input = new GLInputhandler();
	game = new Logic::GameLogic(input);
}

int Linuxhandler::run()
{
	float time = 0;
	timer->Tick();

	do
	{
		//drawing (shall be in render class)
		timer->Tick();
		time += (float)timer->getDelta();

		if(time > 1.f / 600)
		{
			//if(is active window
			game->update(time);

			GraphicsOGL4::getInstance()->draw();

			time = 0;
			// also some updating and shit
			//swap buffers
			glfwSwapBuffers();
		}
	}
	while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS);
	return 0;
}

bool Linuxhandler::initWindow()
{
	// Initialize GLFW
	if(!glfwInit())
	{
		//print error message to console
		return false;
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4); // version 4.3
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Old OpenGL? No thanks!

	if(FULLSCR)
	{
		if(!glfwOpenWindow(SCRWIDTH, SCRHEIGHT, 0, 0, 0, 0, 32, 0, GLFW_FULLSCREEN))
		{
			fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );

			glfwTerminate();
			return false;
		}
	}
	else
	{
		if(!glfwOpenWindow(SCRWIDTH, SCRHEIGHT, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
		{
			fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );

			glfwTerminate();
			return false;
		}
	}
	// initialize GLEW
	glewExperimental = true; // need this for core profile
	if(glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}
	glfwSetWindowTitle("Breakout for dummies");


	//TO CAPTURE THE ESCAPE KEY WHEN IT'S PRESSED
	glfwEnable(GLFW_STICKY_KEYS);

	// dark blue background color
	glClearColor(0.f, 0.f, .4f, 0.f);

	// enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);

	glFrontFace(GL_CW);

    // TODO: If models are clock wise change mode to GL_CW by using glFrontFace(GL_CW);
	return true;
}

void Linuxhandler::createConsoleLog(const char *winTitle)
{
	//here we should create a console log. Need research
}

Linuxhandler::~Linuxhandler()
{
	glfwCloseWindow();

	GraphicsOGL4 *t = GraphicsOGL4::getInstance();

	SAFE_DELETE(t);

	glfwTerminate();
}
#endif
