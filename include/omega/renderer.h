#pragma once
#include <omega/ostd.h>

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

typedef struct OMG_Renderer {
    bool (*init)(struct OMG_Renderer* this);
    bool (*destroy)(struct OMG_Renderer* this);
    int (*get_supported_drivers)(struct OMG_Renderer* this);
    bool (*clear)(struct OMG_Renderer* this, const OMG_Color* col);
    bool (*begin)(struct OMG_Renderer* this);
    bool (*flip)(struct OMG_Renderer* this);
    void (*_on_update_window_size)(struct OMG_Renderer* this);
    void* omg;
    void* win;
    OMG_FPoint scale;
    OMG_FPoint size;
    int driver;
    int type;
    bool inited;
    bool was_allocated;
} OMG_Renderer;

OMG_API void omg_renderer_fill_on_create(OMG_Renderer* this);
OMG_API bool omg_renderer_init(OMG_Renderer* this);
OMG_API bool omg_renderer_destroy(OMG_Renderer* this);
#if OMG_EXPORT_SHIT
OMG_API int omg_renderer_get_supported_drivers(OMG_Renderer* this);
OMG_API bool omg_renderer_clear(OMG_Renderer* this, const OMG_Color* col);
OMG_API void omg_on_update_window_size(OMG_Renderer* this);
OMG_API bool omg_renderer_begin(OMG_Renderer* this);
OMG_API bool omg_renderer_flip(OMG_Renderer* this);
#endif
