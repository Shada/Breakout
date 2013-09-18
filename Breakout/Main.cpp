#ifdef _WIN32
#include <crtdbg.h>
#endif  // _WIN32
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
#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif
}
