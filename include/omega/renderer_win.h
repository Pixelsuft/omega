#pragma once
#include <omega/api_win.h>

#if OMG_SUPPORT_WIN
#include <omega/renderer.h>

typedef struct {
    OMG_Renderer parent;
    PAINTSTRUCT ps;
    HDC hdc;
    HWND hwnd;
    OMG_User32* u32;
    OMG_Gdi32* g32;
    OMG_Dwmapi* dwm;
} OMG_RendererWin;

OMG_API bool omg_renderer_win_init(OMG_RendererWin* this);
OMG_API bool omg_renderer_win_destroy(OMG_RendererWin* this);
#if OMG_EXPORT_SHIT
#include <omega/texture_win.h>

void omg_renderer_win_update_scale(OMG_RendererWin* this);
bool omg_renderer_win_begin(OMG_RendererWin* this);
bool omg_renderer_win_clear(OMG_RendererWin* this, const OMG_Color* col);
bool omg_renderer_win_flip(OMG_RendererWin* this);
#endif
#endif
