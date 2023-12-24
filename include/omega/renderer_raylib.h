#pragma once
#include <omega/api_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/renderer.h>
#define _OMG_RAYLIB_OMG_COLOR(col) ((Color){ .r = (unsigned int)(col)->r, .g = (unsigned int)(col)->g, .b = (unsigned int)(col)->b, .a = (unsigned int)(col)->a })

typedef struct {
    OMG_Renderer parent;
    OMG_Raylib* raylib;
} OMG_RendererRaylib;

OMG_API bool omg_renderer_raylib_init(OMG_RendererRaylib* this);
OMG_API bool omg_renderer_raylib_destroy(OMG_RendererRaylib* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_renderer_raylib_clear(OMG_RendererRaylib* this, const OMG_Color* col);
OMG_API bool omg_renderer_raylib_begin(OMG_RendererRaylib* this);
OMG_API bool omg_renderer_raylib_flip(OMG_RendererRaylib* this);
#endif
#endif
