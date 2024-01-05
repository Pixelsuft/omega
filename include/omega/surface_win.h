#pragma once
#include <omega/api_win.h>

#if OMG_SUPPORT_WIN
#include <omega/surface.h>

typedef struct {
    OMG_Surface parent;
    HDC dc;
    HBITMAP bm;
} OMG_SurfaceWin;
#endif
