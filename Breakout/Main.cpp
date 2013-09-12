#include <crtdbg.h>
#include "Windowhandler.h"


int main()
{
	Windowhandler *wh;
#ifdef WINDOWS
	wh = new Winhandler();
#else
	wh = new Linuxhandler();
#endif

	delete wh;
	wh = NULL;

	_CrtDumpMemoryLeaks();
}
