#pragma once
#include <omega/ostd.h>

typedef struct OMG_Window {
    bool (*default_init)(struct OMG_Window* this);
    bool (*destroy)(struct OMG_Window* this);
    bool (*show)(struct OMG_Window* this);
    void* omg;
    OMG_FPoint size;
    bool inited;
} OMG_Window;

OMG_API bool omg_window_init(OMG_Window* this);
OMG_API bool omg_window_destroy(OMG_Window* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_window_show(OMG_Window* this);
#endif
