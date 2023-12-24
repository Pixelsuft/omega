#include <omega/window_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/omega.h>
#include <omega/renderer_sdl2.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)base->omg)
#define ren_sdl2 ((OMG_RendererSdl2*)base->ren)

bool omg_window_sdl2_show(OMG_WindowSdl2* this, bool show) {
    (show ? this->sdl2->SDL_ShowWindow : this->sdl2->SDL_HideWindow)(this->win);
    return false;
}

bool omg_window_sdl2_set_title(OMG_WindowSdl2* this, const OMG_String* new_title) {
    if (omg_string_ensure_null((OMG_String*)new_title))
        return true;
    this->sdl2->SDL_SetWindowTitle(this->win, new_title->ptr);
    return false;
}

bool omg_window_sdl2_renderer_alloc(OMG_WindowSdl2* this) {
    if (base->ren_type != OMG_REN_TYPE_SDL2)
        base->ren_type = OMG_REN_TYPE_AUTO;
    if (base->ren_type == OMG_REN_TYPE_AUTO) {
        base->ren_type = OMG_REN_TYPE_SDL2;
        bool res = omg_window_sdl2_renderer_alloc(this);
        if (res) {
            base->ren_type = OMG_REN_TYPE_AUTO;
        }
        return res;
    }
    if (base->ren_type == OMG_REN_TYPE_SDL2) {
        base->ren = OMG_MALLOC(omg_base->mem, sizeof(OMG_RendererSdl2));
        if (OMG_ISNULL(base->ren))
            return true;
        base->ren->was_allocated = true;
        base->ren->window = this;
        OMG_BEGIN_POINTER_CAST();
        base->ren->init = omg_renderer_sdl2_init;
        OMG_END_POINTER_CAST();
        ren_sdl2->win = this->win;
        ren_sdl2->sdl2 = this->sdl2;
        return false;
    }
    return true;
}

bool omg_window_sdl2_destroy(OMG_WindowSdl2* this) {
    if (base->inited) {
        omg_window_destroy((OMG_Window*)this);
        this->sdl2->SDL_DestroyWindow(this->win);
        base->inited = false;
    }
    return false;
}

bool omg_window_sdl2_init(OMG_WindowSdl2* this) {
    omg_window_init(base);
    base->type = OMG_WIN_TYPE_SDL2;
    base->inited = false;
    this->win = this->sdl2->SDL_CreateWindow(
        "OMG Window [SDL2]",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        (int)base->size.w, (int)base->size.h,
        SDL_WINDOW_HIDDEN | (omg_base->support_highdpi ? SDL_WINDOW_ALLOW_HIGHDPI : 0)
    );
    if (OMG_ISNULL(this->win)) {
        _OMG_LOG_ERROR(omg_base, "Failed to create SDL2 Window (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    OMG_BEGIN_POINTER_CAST();
    base->renderer_alloc = omg_window_sdl2_renderer_alloc;
    base->show = omg_window_sdl2_show;
    base->set_title = omg_window_sdl2_set_title;
    base->destroy = omg_window_sdl2_destroy;
    OMG_END_POINTER_CAST();
    base->inited = true;
    return false;
}
#endif
