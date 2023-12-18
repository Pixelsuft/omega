#pragma once
#include <omega/config.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/window.h>

typedef struct {
    OMG_Window parent;
} OMG_WindowRaylib;
#endif
