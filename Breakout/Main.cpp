#ifdef _WIN32
#include <crtdbg.h>
#endif  // _WIN32
#include "Windowhandler.h"
#include "Resource.h"
#include <string>

int main(int argc, char* argv[])
{
    // we can get the program path from here. This is good when using paths..
    // have not gotten relative paths to work, so this might be used instead!!

	Windowhandler *wh;
#ifdef _WIN32
	wh = new Winhandler();
#else
	std::string path = argv[0];
#ifdef _DEBUG
	path = path.substr(0, path.length() - 18);
#else
	path = path.substr(0, path.length() - 20);
#endif
	printf("path: %s\n\n", path.c_str());
	ExecPath::linuxPath = path;
	wh = new Linuxhandler();
#endif

	wh->run();

	SAFE_DELETE(wh);
#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif
}
