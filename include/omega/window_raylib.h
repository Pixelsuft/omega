#pragma once
#include <omega/window.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/api_raylib.h>

typedef struct {
    OMG_Window parent;
    OMG_Raylib* raylib;
} OMG_WindowRaylib;

OMG_API bool omg_window_raylib_init(OMG_WindowRaylib* this);
OMG_API bool omg_window_raylib_destroy(OMG_WindowRaylib* this);
#endif