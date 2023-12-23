#include <omega/window_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/omega.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_window_sdl2_show(OMG_WindowSdl2* this) {
    this->sdl2->SDL_ShowWindow(this->win);
    return false;
}

bool omg_window_sdl2_set_title(OMG_WindowSdl2* this, const OMG_String* new_title) {
    if (omg_string_ensure_null((OMG_String*)new_title))
        return true;
    this->sdl2->SDL_SetWindowTitle(this->win, new_title->ptr);
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
    base->show = omg_window_sdl2_show;
    base->set_title = omg_window_sdl2_set_title;
    OMG_END_POINTER_CAST();
    base->inited = true;
    return false;
}

bool omg_window_sdl2_destroy(OMG_WindowSdl2* this) {
    if (base->inited) {
        this->sdl2->SDL_DestroyWindow(this->win);
        base->inited = false;
    }
    omg_window_destroy((OMG_Window*)this);
    return false;
}
#endif
