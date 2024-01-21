#pragma once
#include <omega/api_win.h>

#if OMG_SUPPORT_WIN
#include <omega/renderer.h>

typedef struct {
    OMG_Renderer parent;
    PAINTSTRUCT ps;
    HDC hpdc;
    HDC cur_hpdc;
    HDC hwdc;
    HDC cur_hwdc;
    HWND hwnd;
    OMG_User32* u32;
    OMG_Gdi32* g32;
    OMG_Dwmapi* dwm;
} OMG_RendererWin;

OMG_API bool omg_renderer_win_init(OMG_RendererWin* this);
OMG_API bool omg_renderer_win_destroy(OMG_RendererWin* this);
#if OMG_EXPORT_SHIT
#include <omega/texture_win.h>

OMG_API void omg_renderer_win_update_scale(OMG_RendererWin* this);
OMG_API bool omg_renderer_win_begin(OMG_RendererWin* this);
OMG_API bool omg_renderer_win_clear(OMG_RendererWin* this, const OMG_Color* col);
OMG_API bool omg_renderer_win_flip(OMG_RendererWin* this);
OMG_API bool omg_renderer_win_draw_line(OMG_RendererWin* this, const OMG_FRect* start_end, const OMG_Color* col);
OMG_API bool omg_renderer_win_draw_rect(OMG_RendererWin* this, const OMG_FRect* rect, const OMG_Color* col);
OMG_API bool omg_renderer_win_fill_rect(OMG_RendererWin* this, const OMG_FRect* rect, const OMG_Color* col);
OMG_API bool omg_renderer_win_draw_point(OMG_RendererWin* this, const OMG_FPoint* pos, const OMG_Color* col);
OMG_API bool omg_renderer_win_draw_circle(OMG_RendererWin* this, const OMG_FPoint* pos, float rad, const OMG_Color* col)
OMG_API bool omg_renderer_win_fill_circle(OMG_RendererWin* this, const OMG_FPoint* pos, float rad, const OMG_Color* col);
OMG_API OMG_TextureWin* omg_renderer_win_tex_create(OMG_RendererWin* this, OMG_TextureWin* tex, const OMG_FPoint* size, int access, bool has_alpha);
OMG_API bool omg_renderer_win_tex_destroy(OMG_RendererWin* this, OMG_TextureWin* tex);
OMG_API bool omg_renderer_win_set_target(OMG_RendererWin* this, OMG_TextureWin* tex);
OMG_API bool omg_renderer_win_copy(OMG_RendererWin* this, OMG_TextureWin* tex, const OMG_FPoint* pos);
#endif
#endif
