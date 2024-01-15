#pragma once

#include <globals.h>

void FatalErrorA(char *format, ...);
void FatalErrorW(WCHAR *format, ...);

#define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#if _DEBUG
#define ASSERT(cnd)\
	do {\
		if (!(cnd)){\
			OutputDebugStringA("Assert failed:\n");\
			OutputDebugStringA(#cnd);\
			OutputDebugStringA("\n");\
			__debugbreak();\
			exit(1);\
		}\
	} while (0)
#else
#define ASSERT(cnd)\
	do {\
		if (!(cnd)){\
			FatalErrorA("Assert failed:\nFile: %s\nLine: %d\nCondition: %s",FILENAME,__LINE__,#cnd);\
			exit(1);\
		}\
	} while (0)
#endif