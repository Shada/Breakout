#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#define SAFE_RELEASE(x) if(x) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x)	if(x){ delete(x);		(x) = NULL; }

// temporary, just fr being able to test linux code on win32 environment.
//#define _WIN32

#endif