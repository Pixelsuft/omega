#include <omega/window_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/omega.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)this->omg)

bool omg_window_sdl2_init(OMG_WindowSdl2* this) {
    omg_window_init(base);
    base->inited = false;
    this->win = this->sdl2->SDL_CreateWindow(
        "OMG Window",
        0, 0,
        640, 480,
        0
    );
    if (OMG_ISNULL(this->win)) {
        _OMG_LOG_ERROR(omg_base, "Failed to create window (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    base->inited = true;
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
