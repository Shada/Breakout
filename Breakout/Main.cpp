#include <crtdbg.h>
#include "Windowhandler.h"

#define LINUX 0
#define WINDOWS 1

int main()
{
	Windowhandler *wh;
#if LINUX
	wh = new Linuxhandler();
#endif
#if WINDOWS
	wh = new Winhandler();
#endif

	delete wh;
	wh = NULL;

	_CrtDumpMemoryLeaks();
}
