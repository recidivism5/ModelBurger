#pragma once

#include <error.h>

#ifdef __cplusplus
extern "C" {
#endif

void DwriteInit();
void DwriteConnectTexture(ID3D11Texture2D *texture);
void DwriteReleaseTexture();
void DwriteBegin();
void DwriteEnd();
void DwriteDrawText(WCHAR *str, RECT *r);

#ifdef __cplusplus
}
#endif