#include <omega/api_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/omega.h>
#include <omega/window_raylib.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_window_raylib_init(OMG_WindowRaylib* this) {
    omg_window_init(base);
    base->type = OMG_WIN_TYPE_RAYLIB;
    base->inited = false;
    OMG_BEGIN_POINTER_CAST();
    OMG_END_POINTER_CAST();
    _OMG_LOG_INFO(omg_base, "test");
    this->raylib->InitWindow((int)base->size.w, (int)base->size.h, "OMG Window");
    base->inited = true;
    return false;
}

bool omg_window_raylib_destroy(OMG_WindowRaylib* this) {
    this->raylib->CloseWindow();
    return false;
}
#endif
