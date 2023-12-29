#include <omega/renderer_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/window.h>
#include <omega/omega.h>
#define base ((OMG_Renderer*)this)
#define win_base ((OMG_Window*)base->win)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_renderer_raylib_destroy(OMG_RendererRaylib* this) {
    if (base->inited) {
        base->inited = false;
    }
    return false;
}

bool omg_renderer_raylib_clear(OMG_RendererRaylib* this, const OMG_Color* col) {
    this->raylib->ClearBackground(_OMG_RAYLIB_OMG_COLOR(col));
    return false;
}

bool omg_renderer_raylib_begin(OMG_RendererRaylib* this) {
    this->raylib->BeginDrawing();
    return false;
}

bool omg_renderer_raylib_flip(OMG_RendererRaylib* this) {
    this->raylib->EndDrawing();
    return false;
}

int omg_renderer_raylib_get_supported_drivers(OMG_RendererRaylib* this) {
    OMG_UNUSED(this);
    return OMG_REN_DRIVER_OPENGL;
}

bool omg_renderer_raylib_init(OMG_RendererRaylib* this) {
    OMG_BEGIN_POINTER_CAST();
    omg_renderer_init(this);
    base->destroy = omg_renderer_raylib_destroy;
    base->clear = omg_renderer_raylib_clear;
    base->begin = omg_renderer_raylib_begin;
    base->flip = omg_renderer_raylib_flip;
    OMG_END_POINTER_CAST();
    base->type = OMG_REN_TYPE_RAYLIB;
    base->inited = true;
    _OMG_LOG_INFO(omg_base, "Raylib renderer created successfuly with opengl driver");
    return false;
}
#endif
