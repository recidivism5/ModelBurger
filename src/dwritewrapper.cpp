#include <dwritewrapper.h>

#include <d2d1.h>
#include <dwrite.h>

extern "C" {
	
	static ID2D1Factory *d2dFactory;
	static ID2D1RenderTarget *d2dRenderTarget;

	static IDWriteFactory *dwFactory;
	static IDWriteTextFormat *dwTextFormat;

	void DwriteInit(){
		if (!d2dFactory){
			ASSERT(SUCCEEDED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,__uuidof(ID2D1Factory),(void **)&d2dFactory)));
		}
		if (!dwFactory){
			ASSERT(SUCCEEDED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown **)&dwFactory)));
		}

		ASSERT(SUCCEEDED(dwFactory->CreateTextFormat(
			L"Segoe-UI",
			0,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			12.0f,
			L"",
			&dwTextFormat
		)));
	}

	void DwriteConnectTexture(ID3D11Texture2D *texture){
		IDXGISurface *surface = 0;
		ASSERT(SUCCEEDED(texture->QueryInterface(__uuidof(IDXGISurface), (void **)&surface)));

		float dpi = (float)GetDpiForWindow(gwnd);

		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
				dpi,
				dpi
		);
		DwriteReleaseTexture();
		ASSERT(SUCCEEDED(d2dFactory->CreateDxgiSurfaceRenderTarget(surface,&props,&d2dRenderTarget)));

		d2dRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

		surface->Release();
	}

	void DwriteReleaseTexture(){
		if (d2dRenderTarget){
			d2dRenderTarget->Release();
			d2dRenderTarget = 0;
		}
	}

	void DwriteBegin(){
		d2dRenderTarget->BeginDraw();
	}

	void DwriteEnd(){
		d2dRenderTarget->EndDraw(0,0);
	}

	void DwriteDrawText(WCHAR *str, int x, int y){
		RECT cr;
		GetClientRect(gwnd,&cr);

		ID2D1SolidColorBrush *brush = 0;
		ASSERT(SUCCEEDED(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f,1.0f,1.0f,1.0f),&brush)));

		d2dRenderTarget->DrawTextW(
			str,
			(UINT32)wcslen(str),
			dwTextFormat,
			D2D1::RectF((float)cr.left,(float)cr.top,(float)cr.right,(float)cr.bottom),
			brush,
			D2D1_DRAW_TEXT_OPTIONS_NONE,
			DWRITE_MEASURING_MODE_NATURAL
		);
			
		brush->Release();
	}

}