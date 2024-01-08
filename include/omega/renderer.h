#pragma once
#include <omega/ostd.h>
#include <omega/surface.h>
#include <omega/texture.h>

#if OMG_NULL_TEX_CHECK
#define _OMG_NULL_TEX_CHECK() do { \
    if (OMG_ISNULL(tex)) \
        return true; \
} while (0)
#else
#define _OMG_NULL_TEX_CHECK()
#endif

#define OMG_REN_TYPE_NONE 0
#define OMG_REN_TYPE_AUTO 1
#define OMG_REN_TYPE_WIN 2
#define OMG_REN_TYPE_SDL2 3
#define OMG_REN_TYPE_RAYLIB 4

#define OMG_REN_DRIVER_NONE 0
#define OMG_REN_DRIVER_AUTO (1 << 1)
#define OMG_REN_DRIVER_D3D9 (1 << 2)
#define OMG_REN_DRIVER_D3D11 (1 << 3)
#define OMG_REN_DRIVER_D3D12 (1 << 4)
#define OMG_REN_DRIVER_OPENGL (1 << 5)
#define OMG_REN_DRIVER_OPENGLES (1 << 6)
#define OMG_REN_DRIVER_OPENGLES2 (1 << 7)
#define OMG_REN_DRIVER_VULKAN (1 << 8)
#define OMG_REN_DRIVER_RESERVED (1 << 9)
#define OMG_REN_DRIVER_SOFTWARE (1 << 10)
#define OMG_REN_DRIVER_UNKNOWN (1 << 11)

typedef struct OMG_Renderer {
    void (*_on_update_window_size)(struct OMG_Renderer* this);
    bool (*init)(struct OMG_Renderer* this);
    bool (*destroy)(struct OMG_Renderer* this);
    int (*get_supported_drivers)(struct OMG_Renderer* this);
    bool (*clear)(struct OMG_Renderer* this, const OMG_Color* col);
    bool (*begin)(struct OMG_Renderer* this);
    bool (*flip)(struct OMG_Renderer* this);
    bool (*set_scale)(struct OMG_Renderer* this, const OMG_FPoint* offset, const OMG_FPoint* scale);
    bool (*set_target)(struct OMG_Renderer* this, OMG_Texture* tex);
    bool (*draw_point)(struct OMG_Renderer* this, const OMG_FPoint* pos, const OMG_Color* col);
    bool (*draw_line)(struct OMG_Renderer* this, const OMG_FRect* start_end, const OMG_Color* col);
    bool (*draw_rect)(struct OMG_Renderer* this, const OMG_FRect* rect, const OMG_Color* col);
    bool (*fill_rect)(struct OMG_Renderer* this, const OMG_FRect* rect, const OMG_Color* col);
    bool (*draw_circle)(struct OMG_Renderer* this, const OMG_FPoint* pos, float rad, const OMG_Color* col);
    bool (*fill_circle)(struct OMG_Renderer* this, const OMG_FPoint* pos, float rad, const OMG_Color* col);
    OMG_Texture* (*tex_from_surf)(struct OMG_Renderer* this, OMG_Surface* surf, bool destroy_surf);
    OMG_Texture* (*tex_create)(struct OMG_Renderer* this, const OMG_FPoint* size, int access, bool has_alpha);
    bool (*tex_destroy)(struct OMG_Renderer* this, OMG_Texture* tex);
    bool (*copy)(struct OMG_Renderer* this, OMG_Texture* tex, const OMG_FPoint* pos);
    void* omg;
    void* win;
    OMG_Texture* target;
    OMG_FPoint scale;
    OMG_FPoint dpi_scale;
    OMG_FPoint size;
    OMG_FPoint offset;
    OMG_Color color;
    float a_scale;
    int driver;
    int type;
    bool aa;
    bool soft_scale;
    bool auto_blend;
    bool inited;
    bool was_allocated;
} OMG_Renderer;

OMG_API void omg_renderer_fill_on_create(OMG_Renderer* this);
OMG_API bool omg_renderer_init(OMG_Renderer* this);
OMG_API bool omg_renderer_destroy(OMG_Renderer* this);
OMG_API bool omg_renderer_set_scale(OMG_Renderer* this, const OMG_FPoint* offset, const OMG_FPoint* scale);
#if OMG_EXPORT_SHIT
OMG_API int omg_renderer_get_supported_drivers(OMG_Renderer* this);
OMG_API bool omg_renderer_clear(OMG_Renderer* this, const OMG_Color* col);
OMG_API void omg_on_update_window_size(OMG_Renderer* this);
OMG_API bool omg_renderer_begin(OMG_Renderer* this);
OMG_API bool omg_renderer_flip(OMG_Renderer* this);
OMG_API bool omg_renderer_set_target(OMG_Renderer* this, OMG_Texture* tex);
OMG_API bool omg_renderer_draw_point(OMG_Renderer* this, const OMG_FPoint* pos, const OMG_Color* col);
OMG_API bool omg_renderer_draw_fill_rect(OMG_Renderer* this, const OMG_FRect* rect, const OMG_Color* col);
OMG_API bool omg_renderer_draw_circle(OMG_Renderer* this, const OMG_FPoint* pos, float rad, const OMG_Color* col);
OMG_API OMG_Texture* omg_renderer_tex_from_surf(OMG_Renderer* this, OMG_Surface* surf, bool destroy_surf);
OMG_API OMG_Texture* omg_renderer_tex_create(OMG_Renderer* this, const OMG_FPoint* size, int access, bool has_alpha);
OMG_API bool omg_renderer_tex_destroy(OMG_Renderer* this, OMG_Texture* tex);
OMG_API bool omg_renderer_copy(OMG_Renderer* this, OMG_Texture* tex, const OMG_FPoint* pos);
#endif
