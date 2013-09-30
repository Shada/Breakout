#ifdef BAJSAPA
#include <crtdbg.h>
#endif  // BAJSAPA
#include "Windowhandler.h"
#include "Resource.h"


int main(int argc, char* argv[])
{
    // we can get the program path from here. This is good when using paths..
    // have not gotten relative paths to work, so this might be used instead!!
    printf("running: %s\n\n", argv[0]);
	Windowhandler *wh;
#ifdef BAJSAPA
	wh = new Winhandler();
#else
	wh = new Linuxhandler();
#endif

	wh->run();

	SAFE_DELETE(wh);
#ifdef BAJSAPA
	_CrtDumpMemoryLeaks();
#endif
}
