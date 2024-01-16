#pragma once

#include <globals.h>

#ifdef __cplusplus
extern "C" {
#endif

char *GetWinErrorA();
WCHAR *GetWinErrorW();

void FatalErrorA(char *format, ...);
void FatalErrorW(WCHAR *format, ...);

#ifdef __cplusplus
}
#endif

#define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#if _DEBUG
#define ASSERT(cnd)\
	do {\
		if (!(cnd)){\
			OutputDebugStringA("\nAssert failed:\n");\
			OutputDebugStringA(#cnd);\
			OutputDebugStringA("\nLast Windows error:\n");\
			OutputDebugStringA(GetWinErrorA());\
			OutputDebugStringA("\n");\
			__debugbreak();\
			exit(1);\
		}\
	} while (0)
#else
#define ASSERT(cnd)\
	do {\
		if (!(cnd)){\
			FatalErrorA("Assert failed:\nFile: %s\nLine: %d\nCondition: %s\nLast Windows error: %s",FILENAME,__LINE__,#cnd,GetWinErrorA());\
			exit(1);\
		}\
	} while (0)
#endif