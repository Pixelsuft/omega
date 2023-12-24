#pragma once
#include <omega/ostd.h>

#define OMG_REN_TYPE_NONE 0
#define OMG_REN_TYPE_AUTO 1
#define OMG_REN_TYPE_WIN 2
#define OMG_REN_TYPE_SDL2 3

typedef struct OMG_Renderer {
    bool (*init)(struct OMG_Renderer* this);
    bool (*destroy)(struct OMG_Renderer* this);
    bool (*clear)(struct OMG_Renderer* this, const OMG_Color* col);
    bool (*flip)(struct OMG_Renderer* this);
    void* omg;
    void* win;
    int type;
    bool inited;
    bool was_allocated;
} OMG_Renderer;

OMG_API bool omg_renderer_init(OMG_Renderer* this);
OMG_API bool omg_renderer_destroy(OMG_Renderer* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_renderer_clear(OMG_Renderer* this, const OMG_Color* col);
OMG_API bool omg_renderer_flip(OMG_Renderer* this);
#endif
