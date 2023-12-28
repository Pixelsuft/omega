#include <omega/api_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/omega.h>
#include <omega/window_raylib.h>
#include <omega/renderer_raylib.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)base->omg)
#define ren_raylib ((OMG_RendererRaylib*)base->ren)

bool omg_window_raylib_show(OMG_WindowRaylib* this, bool show) {
    (show ? this->raylib->ClearWindowState : this->raylib->SetWindowState)(FLAG_WINDOW_HIDDEN);
    return false;
}

bool omg_window_raylib_destroy(OMG_WindowRaylib* this) {
    if (base->inited) {
        omg_window_destroy((OMG_Window*)this);
        this->raylib->CloseWindow();
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

bool omg_window_raylib_renderer_alloc(OMG_WindowRaylib* this) {
    base->ren_type = OMG_REN_TYPE_RAYLIB;
    base->ren = OMG_MALLOC(omg_base->mem, sizeof(OMG_RendererRaylib));
    if (OMG_ISNULL(base->ren))
        return true;
    omg_renderer_fill_on_create(base->ren);
    base->ren->was_allocated = true;
    base->ren->win = this;
    OMG_BEGIN_POINTER_CAST();
    base->ren->init = omg_renderer_raylib_init;
    OMG_END_POINTER_CAST();
    ren_raylib->raylib = this->raylib;
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
    base->renderer_alloc = omg_window_raylib_renderer_alloc;
    OMG_END_POINTER_CAST();
    this->raylib->SetConfigFlags(
        FLAG_WINDOW_HIDDEN |
        (omg_base->support_highdpi ? FLAG_WINDOW_HIGHDPI : 0) |
        (base->vsync ? FLAG_VSYNC_HINT : 0)
    );
    this->raylib->InitWindow((int)base->size.w, (int)base->size.h, "OMG Window [Raylib]");
    base->inited = this->raylib->IsWindowReady();
    return !base->inited;
}
#endif
