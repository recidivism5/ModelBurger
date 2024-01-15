//standard library includes:
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <intrin.h>

//windows includes:
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define UNICODE
#define COBJMACROS
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dwmapi.h>

//local includes:
#include "res.h"

void FatalErrorA(char *format, ...){
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
#define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#if _DEBUG
#define ASSERT(cnd)\
	do {\
		if (!(cnd)){\
			__debugbreak();\
			exit(1);\
		}\
	} while (0)
#else
#define ASSERT(cnd)\
	do {\
		if (!(cnd)){\
			FatalErrorA(FILENAME,__LINE__,#cnd);\
			exit(1);\
		}\
	} while (0)
#endif

void *MallocOrDie(size_t size){
	void *p = malloc(size);
	ASSERT(p);
	return p;
}
void *ZallocOrDie(size_t size){
	void *p = calloc(1,size);
	ASSERT(p);
	return p;
}
void *ReallocOrDie(void *ptr, size_t size){
	void *p = realloc(ptr,size);
	ASSERT(p);
	return p;
}

IDXGISwapChain *swapChain;
ID3D11Device *device;
ID3D11DeviceContext *deviceContext;
ID3D11RenderTargetView *renderTargetView;
ID3D11DepthStencilView *depthStencilView;

void CreateRenderTargets(){
	ID3D11Texture2D *backBuffer;
	ASSERT(SUCCEEDED(swapChain->lpVtbl->GetBuffer(swapChain, 0, &IID_ID3D11Texture2D, (void**)&backBuffer)));
	ASSERT(SUCCEEDED(device->lpVtbl->CreateRenderTargetView(device, (ID3D11Resource *)backBuffer, 0, &renderTargetView)));

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	backBuffer->lpVtbl->GetDesc(backBuffer, &depthStencilDesc);

	backBuffer->lpVtbl->Release(backBuffer);

	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D* depthStencil;

	device->lpVtbl->CreateTexture2D(device, &depthStencilDesc, 0, &depthStencil);
	device->lpVtbl->CreateDepthStencilView(device, (ID3D11Resource *)depthStencil, 0, &depthStencilView);

	depthStencil->lpVtbl->Release(depthStencil);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
	switch(msg){
		case WM_CREATE:{
			DWORD darkTitlebar = 1;
			int DwmwaUseImmersiveDarkMode = 20,
				DwmwaUseImmersiveDarkModeBefore20h1 = 19;
			SUCCEEDED(DwmSetWindowAttribute(hwnd, DwmwaUseImmersiveDarkMode, &darkTitlebar, sizeof(darkTitlebar))) ||
				SUCCEEDED(DwmSetWindowAttribute(hwnd, DwmwaUseImmersiveDarkModeBefore20h1, &darkTitlebar, sizeof(darkTitlebar)));

			RECT wr;
			GetWindowRect(hwnd,&wr);
			SetWindowPos(hwnd,0,wr.left,wr.top,wr.right-wr.left,wr.bottom-wr.top,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE); //prevent initial white frame

			UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG_BUILD)
			creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0};
			DXGI_SWAP_CHAIN_DESC swapChainDesc = {
				.BufferDesc.Width = 0,
				.BufferDesc.Height = 0,
				.BufferDesc.RefreshRate.Numerator = 0,
				.BufferDesc.RefreshRate.Denominator = 0,
				.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
				.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
				.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED,

				.SampleDesc.Count = 1,
				.SampleDesc.Quality = 0,

				.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
				.BufferCount = 2,

				.OutputWindow = hwnd,
				.Windowed = TRUE,
				.SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
				.Flags = 0
			};
			ASSERT(SUCCEEDED(D3D11CreateDeviceAndSwapChain(
				0,
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				creationFlags,
				featureLevels,
				ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION,
				&swapChainDesc,
				&swapChain,
				&device,
				0,
				&deviceContext
			)));

			CreateRenderTargets();
			break;
		}
		case WM_PAINT:{
			deviceContext->lpVtbl->OMSetRenderTargets(deviceContext, 1, &renderTargetView, depthStencilView);
			FLOAT clearColor[4] = {0.1f, 0.2f, 0.6f, 1.0f};
			deviceContext->lpVtbl->ClearRenderTargetView(deviceContext, renderTargetView, clearColor);
			deviceContext->lpVtbl->ClearDepthStencilView(deviceContext, depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
			swapChain->lpVtbl->Present(swapChain, 1, 0);
			return 0;
		}
		case WM_DESTROY:{
			PostQuitMessage(0);
			break;
		}
		case WM_SIZE:{
			deviceContext->lpVtbl->OMSetRenderTargets(deviceContext,0,0,0);
			renderTargetView->lpVtbl->Release(renderTargetView);
			depthStencilView->lpVtbl->Release(depthStencilView);
			ASSERT(SUCCEEDED(swapChain->lpVtbl->ResizeBuffers(swapChain,0,0,0,DXGI_FORMAT_UNKNOWN,0)));
			CreateRenderTargets();
			break;
		}
	}
	return DefWindowProcW(hwnd, msg, wparam, lparam);
}

int main(int argc, char **argv){
	WNDCLASSEXW wcex = {
		.cbSize = sizeof(wcex),
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = WndProc,
		.hInstance = GetModuleHandleW(0),
		.hIcon = LoadIconW(GetModuleHandleW(0),MAKEINTRESOURCEW(RID_ICON)),
		.hCursor = LoadCursorW(0,IDC_ARROW),
		.lpszClassName = L"Bruh Modeler",
		.hIconSm = 0,
	};
	ASSERT(RegisterClassExW(&wcex));

	int width = 640;
	int height = 480;

	RECT initialRect = {0, 0, width, height};
	AdjustWindowRect(&initialRect,WS_OVERLAPPEDWINDOW,FALSE);
	LONG initialWidth = initialRect.right - initialRect.left;
	LONG initialHeight = initialRect.bottom - initialRect.top;

	HWND hwnd = CreateWindowExW(
		0, //WS_EX_OVERLAPPEDWINDOW fucks up the borders when maximized
		wcex.lpszClassName,
		wcex.lpszClassName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		GetSystemMetrics(SM_CXSCREEN)/2-initialWidth/2,
		GetSystemMetrics(SM_CYSCREEN)/2-initialHeight/2,
		initialWidth, 
		initialHeight,
		0, 0, wcex.hInstance, 0);
	ASSERT(hwnd);

	MSG msg;
	while (GetMessageW(&msg,0,0,0)){
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 0;
}
