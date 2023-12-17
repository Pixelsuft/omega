#include <omega/window.h>

void omg_window_fill_on_create(OMG_Window* this) {
    this->type = OMG_WIN_TYPE_NONE;
    this->size.w = 800.0f;
    this->size.h = 600.0f;
}

bool omg_window_show(OMG_Window* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_window_init(OMG_Window* this) {
    this->destroy = omg_window_destroy;
    this->show = omg_window_show;
    return false;
}

bool omg_window_destroy(OMG_Window* this) {
    OMG_UNUSED(this);
    return false;
}
