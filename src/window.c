#include <omega/window.h>

bool omg_window_init(OMG_Window* this) {
    this->destroy = omg_window_destroy;
    return false;
}

bool omg_window_destroy(OMG_Window* this) {
    OMG_UNUSED(this);
    return false;
}
