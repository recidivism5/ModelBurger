#include <error.h>

void FatalErrorA(char *format, ...){
	ValidateRect(gwnd,0); //the messagebox doesn't work unless we stop the window from trying to paint itself

	va_list args;
	va_start(args,format);
	static char msg[1024];
	vsnprintf(msg,ARRAYSIZE(msg),format,args);
#if _DEBUG
	OutputDebugStringA(msg);
	__debugbreak();
#else
	MessageBoxA(0,msg,"Error",MB_ICONERROR);
#endif
	va_end(args);
	exit(1);
}

void FatalErrorW(WCHAR *format, ...){
	ValidateRect(gwnd,0); //the messagebox doesn't work unless we stop the window from trying to paint itself

	va_list args;
	va_start(args,format);
	static WCHAR msg[1024];
	vswprintf(msg,ARRAYSIZE(msg),format,args);
#if _DEBUG
	OutputDebugStringW(msg);
	__debugbreak();
#else
	MessageBoxW(0,msg,L"Error",MB_ICONERROR);
#endif
	va_end(args);
	exit(1);
}