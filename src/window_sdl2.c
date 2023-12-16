#include <omega/window_sdl2.h>

#if OMG_SUPPORT_SDL2
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)this->omg)

bool omg_window_sdl2_init(OMG_WindowSdl2* this) {
    omg_window_init(base);
    base->inited = false;
    return false;
}

bool omg_window_sdl2_destroy(OMG_WindowSdl2* this) {
    OMG_UNUSED(this);
    base->inited = false;
    return false;
}
#endif
