#include <error.h>

char *GetWinErrorA(){
    DWORD dw = GetLastError(); 

    char *msg;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&msg,
        0, NULL);

    return msg;
}

WCHAR *GetWinErrorW(){
    DWORD dw = GetLastError(); 

    WCHAR *msg;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&msg,
        0, NULL);

    return msg;
}

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