#include <error.h>

void FatalErrorA(char *format, ...){
	ValidateRect(gwnd,0);

	va_list args;
	va_start(args,format);
	static char msg[1024];
	vsprintf(msg,format,args);
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
	ValidateRect(gwnd,0);

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