#include <omega/window_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/omega.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)this->omg)

bool omg_window_sdl2_show(OMG_WindowSdl2* this) {
    this->sdl2->SDL_ShowWindow(this->win);
    return false;
}

bool omg_window_sdl2_init(OMG_WindowSdl2* this) {
    omg_window_init(base);
    base->inited = false;
    this->win = this->sdl2->SDL_CreateWindow(
        "OMG Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        (int)base->size.w, (int)base->size.h,
        SDL_WINDOW_HIDDEN | (omg_base->support_highdpi ? SDL_WINDOW_ALLOW_HIGHDPI : 0)
    );
    if (OMG_ISNULL(this->win)) {
        _OMG_LOG_ERROR(omg_base, "Failed to create SDL2 Window (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    base->inited = true;
    OMG_BEGIN_POINTER_CAST();
    base->show = omg_window_sdl2_show;
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_window_sdl2_destroy(OMG_WindowSdl2* this) {
    OMG_UNUSED(this);
    if (base->inited) {
        this->sdl2->SDL_DestroyWindow(this->win);
        base->inited = false;
    }
    return false;
}
#endif
