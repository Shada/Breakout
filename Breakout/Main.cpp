#ifndef _WIN32
#include <crtdbg.h>
#endif  // _WIN32
#include "Windowhandler.h"
#include "Resource.h"


int main(int argc, char* argv[])
{
    // we can get the program path from here. This is good when using paths..
    // have not gotten relative paths to work, so this might be used instead!!
    printf("running: %s\n\n", argv[0]);
	Windowhandler *wh;
#ifdef _WIN32
	wh = new Winhandler();
#else
	wh = new Linuxhandler();
#endif

	wh->run();

	SAFE_DELETE(wh);
#ifndef _WIN32
	_CrtDumpMemoryLeaks();
#endif
}
