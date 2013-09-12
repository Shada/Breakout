#include <crtdbg.h>
#include "Windowhandler.h"
#include "Resource.h"

int main()
{
	Windowhandler *wh;
#ifdef _WIN32
	wh = new Winhandler();
#else
	wh = new Linuxhandler();
#endif

	wh->run();

	SAFE_DELETE(wh);

	_CrtDumpMemoryLeaks();
}
