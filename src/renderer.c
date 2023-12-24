#include <omega/renderer.h>
#include <omega/window.h>
#include <omega/omega.h>
#define win ((OMG_Window*)this->window)
#define omg_base ((OMG_Omega*)this->omg)

bool omg_renderer_destroy(OMG_Renderer* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_renderer_init(OMG_Renderer* this) {
    this->type = OMG_REN_TYPE_NONE;
    this->inited = false;
    this->omg = win->omg;
    this->destroy = omg_renderer_destroy;
    return false;
}
