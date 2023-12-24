#include <omega/api_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/omega.h>
#include <omega/window_raylib.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_window_raylib_show(OMG_WindowRaylib* this, bool show) {
    (show ? this->raylib->ClearWindowState : this->raylib->SetWindowState)(FLAG_WINDOW_HIDDEN);
    return false;
}

bool omg_window_raylib_destroy(OMG_WindowRaylib* this) {
    if (base->inited) {
        this->raylib->CloseWindow();
        omg_window_destroy((OMG_Window*)this);
        base->inited = false;
    }
    return false;
}

bool omg_window_raylib_set_title(OMG_WindowRaylib* this, const OMG_String* new_title) {
    if (omg_string_ensure_null((OMG_String*)new_title))
        return true;
    this->raylib->SetWindowTitle(new_title->ptr);
    return false;
}

bool omg_window_raylib_init(OMG_WindowRaylib* this) {
    omg_window_init(base);
    base->type = OMG_WIN_TYPE_RAYLIB;
    base->inited = false;
    OMG_BEGIN_POINTER_CAST();
    base->show = omg_window_raylib_show;
    base->set_title = omg_window_raylib_set_title;
    base->destroy = omg_window_raylib_destroy;
    OMG_END_POINTER_CAST();
    this->raylib->SetConfigFlags(FLAG_WINDOW_HIDDEN | (omg_base->support_highdpi ? FLAG_WINDOW_HIGHDPI : 0));
    this->raylib->InitWindow((int)base->size.w, (int)base->size.h, "OMG Window [Raylib]");
    base->inited = this->raylib->IsWindowReady();
    return !base->inited;
}
#endif
