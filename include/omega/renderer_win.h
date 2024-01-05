#pragma once
#include <omega/api_win.h>

#if OMG_SUPPORT_WIN
#include <omega/renderer.h>

typedef struct {
    OMG_Renderer parent;
    HDC hdc;
    HWND hwnd;
    OMG_User32* u32;
    OMG_Gdi32* g32;
} OMG_RendererWin;

OMG_API bool omg_renderer_win_init(OMG_RendererWin* this);
OMG_API bool omg_renderer_win_destroy(OMG_RendererWin* this);
#if OMG_EXPORT_SHIT
#include <omega/texture_win.h>

#endif
#endif
