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
    this->u32->ReleaseDC(this->hwnd, this->hdc);
    this->hdc = NULL;
    return false;
}

bool omg_renderer_win_init(OMG_RendererWin* this) {
    this->hdc = this->u32->GetDC(this->hwnd);
    OMG_BEGIN_POINTER_CAST();
    omg_renderer_init(this);
    base->_on_update_window_size = omg_renderer_win_update_scale;
    base->destroy = omg_renderer_win_destroy;
    OMG_END_POINTER_CAST();
    base->type = OMG_REN_TYPE_RAYLIB;
    base->inited = true;
    omg_renderer_win_update_scale(this);
    _OMG_LOG_INFO(omg_base, "Win32 renderer created successfuly");
    return false;
}
#endif
