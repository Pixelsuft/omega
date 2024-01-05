#include <omega/renderer_win.h>

#if OMG_SUPPORT_WIN
#include <omega/omega.h>
#include <omega/texture_win.h>
#define base ((OMG_Renderer*)this)
#define tex_base ((OMG_Texture*)tex)
#define win_base ((OMG_Window*)base->win)
#define omg_base ((OMG_Omega*)base->omg)

void omg_renderer_win_update_scale(OMG_RendererWin* this) {
    if (omg_base->support_highdpi) {
        base->dpi_scale.x = win_base->scale.w;
        base->dpi_scale.y = win_base->scale.y;
    }
    base->size.w = win_base->size.w * base->dpi_scale.x;
    base->size.h = win_base->size.h * base->dpi_scale.y;
}

bool omg_renderer_win_destroy(OMG_RendererWin* this) {
    this->hdc = NULL;
    return false;
}

bool omg_renderer_win_begin(OMG_RendererWin* this) {
    this->hdc = this->u32->BeginPaint(this->hwnd, &this->ps);
    return OMG_ISNULL(this->hdc);
}

bool omg_renderer_win_clear(OMG_RendererWin* this, const OMG_Color* col) {
    HBRUSH brush = this->g32->CreateSolidBrush(RGB(0, 0, 0));
    this->g32->SelectObject(this->hdc, (HGDIOBJ)brush);
    return false;
}

bool omg_renderer_win_flip(OMG_RendererWin* this) {
    bool res = false;
    this->u32->EndPaint(this->hwnd, &this->ps);
    if (OMG_ISNOTNULL(this->dwm->DwmFlush))
        this->dwm->DwmFlush();
    return res;
}

bool omg_renderer_win_init(OMG_RendererWin* this) {
    OMG_BEGIN_POINTER_CAST();
    omg_renderer_init(this);
    base->_on_update_window_size = omg_renderer_win_update_scale;
    base->destroy = omg_renderer_win_destroy;
    base->begin = omg_renderer_win_begin;
    base->clear = omg_renderer_win_clear;
    base->flip = omg_renderer_win_flip;
    OMG_END_POINTER_CAST();
    base->type = OMG_REN_TYPE_RAYLIB;
    base->inited = true;
    omg_renderer_win_update_scale(this);
    _OMG_LOG_INFO(omg_base, "Win32 renderer created successfuly");
    return false;
}
#endif
