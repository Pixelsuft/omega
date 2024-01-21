#pragma once
#include <omega/api_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/renderer.h>
#define _OMG_RAYLIB_OMG_COLOR(col) ((Color){ \
    .r = (unsigned int)((col)->r * (omg_color_t)255 / OMG_MAX_COLOR),\
    .g = (unsigned int)((col)->g * (omg_color_t)255 / OMG_MAX_COLOR),\
    .b = (unsigned int)((col)->b * (omg_color_t)255 / OMG_MAX_COLOR), \
    .a = (unsigned int)((col)->a * (omg_color_t)255 / OMG_MAX_COLOR) \
})

typedef struct {
    OMG_Renderer parent;
    OMG_Raylib* raylib;
    OMG_FPoint ss;
    OMG_FPoint so;
    int blend_cache;
} OMG_RendererRaylib;

OMG_API bool omg_renderer_raylib_init(OMG_RendererRaylib* this);
OMG_API bool omg_renderer_raylib_destroy(OMG_RendererRaylib* this);
OMG_API int omg_renderer_raylib_get_supported_drivers(OMG_RendererRaylib* this);
#if OMG_EXPORT_SHIT
#include <omega/surface_raylib.h>
#include <omega/texture_raylib.h>

OMG_API void omg_renderer_raylib_update_scale(OMG_RendererRaylib* this);
OMG_API bool omg_renderer_raylib_clear(OMG_RendererRaylib* this, const OMG_Color* col);
OMG_API bool omg_renderer_raylib_begin(OMG_RendererRaylib* this);
OMG_API bool omg_renderer_raylib_flip(OMG_RendererRaylib* this);
OMG_API bool omg_renderer_raylib_set_scale(OMG_RendererRaylib* this, const OMG_FPoint* offset, const OMG_FPoint* scale);
OMG_API bool omg_renderer_raylib_set_target(OMG_RendererRaylib* this, OMG_TextureRaylib* tex);
OMG_API bool omg_renderer_raylib_draw_point(OMG_RendererRaylib* this, const OMG_FPoint* pos, const OMG_Color* col);
OMG_API bool omg_renderer_raylib_draw_line(OMG_RendererRaylib* this, const OMG_FRect* start_end, const OMG_Color* col);
OMG_API bool omg_renderer_raylib_draw_rect(OMG_RendererRaylib* this, const OMG_FRect* rect, const OMG_Color* col);
OMG_API bool omg_renderer_raylib_fill_rect(OMG_RendererRaylib* this, const OMG_FRect* rect, const OMG_Color* col);
OMG_API bool omg_renderer_raylib_draw_circle(OMG_RendererRaylib* this, const OMG_FPoint* pos, float rad, const OMG_Color* col);
OMG_API bool omg_renderer_raylib_fill_circle(OMG_RendererRaylib* this, const OMG_FPoint* pos, float rad, const OMG_Color* col);
OMG_API OMG_TextureRaylib* omg_renderer_raylib_tex_from_surf(OMG_RendererRaylib* this, OMG_TextureRaylib* tex, OMG_SurfaceRaylib* surf, bool destroy_surf);
OMG_API OMG_TextureRaylib* omg_renderer_raylib_tex_create(OMG_RendererRaylib* this, OMG_TextureRaylib* tex, const OMG_FPoint* size, int access, bool has_alpha);
OMG_API bool omg_renderer_raylib_tex_destroy(OMG_RendererRaylib* this, OMG_TextureRaylib* tex);
OMG_API bool omg_renderer_raylib_copy(OMG_RendererRaylib* this, OMG_TextureRaylib* tex, const OMG_FPoint* pos);
OMG_API bool omg_renderer_raylib_copy_ex(OMG_RendererRaylib* this, OMG_TextureRaylib* tex, const OMG_FRect* src, const OMG_FRect* dst, const OMG_FPoint* origin, const double rot);
OMG_API bool omg_renderer_raylib_tex_set_color_mod(OMG_RendererRaylib* this, OMG_TextureRaylib* tex, const OMG_Color* col);
OMG_API bool omg_renderer_raylib_set_blend_mode(OMG_RendererRaylib* this, int blend_mode);
#endif
#endif
