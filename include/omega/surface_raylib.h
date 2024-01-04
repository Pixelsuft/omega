#pragma once
#include <omega/api_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/surface.h>

typedef struct {
    OMG_Surface parent;
    Image img;
} OMG_SurfaceRaylib;
#endif
