#pragma once
#include <omega/api_win.h>

#if OMG_SUPPORT_WIN
#include <omega/texture.h>

typedef struct {
    OMG_Texture parent;
    HDC dc;
    HBITMAP bm;
} OMG_TextureWin;
#endif
