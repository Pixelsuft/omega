#pragma once
#include <omega/ostd.h>

#define OMG_FONT_MGR_NONE 0
#define OMG_FONT_MGR_SDL2 0
#define OMG_FONT_MGR_RAYLIB 0

typedef struct {
    bool was_allocated;
} OMG_Font;

typedef struct OMG_FontMgr {
    bool (*init)(struct OMG_FontMgr* this);
    bool (*destroy)(struct OMG_FontMgr* this);
    void* omg;
    bool was_allocated;
    bool inited;
} OMG_FontMgr;

OMG_API bool omg_fontmgr_init(OMG_FontMgr* this);
OMG_API bool omg_fontmgr_destroy(OMG_FontMgr* this);
#if OMG_EXPORT_SHIT
#endif
