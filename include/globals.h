#pragma once

//standard library includes:
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

//windows includes:
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define UNICODE
#define COBJMACROS
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dwmapi.h>

#ifdef __cplusplus
extern "C" {
#endif

extern HWND gwnd;

extern IDXGISwapChain *swapChain;
extern ID3D11Device *device;
extern ID3D11DeviceContext *deviceContext;
extern ID3D11RenderTargetView *renderTargetView;
extern ID3D11DepthStencilView *depthStencilView;

#ifdef __cplusplus
}
#endif