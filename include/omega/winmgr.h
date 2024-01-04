#pragma once
#include <omega/ostd.h>
#include <omega/window.h>
#include <omega/surface.h>

typedef struct OMG_Winmgr {
    void* omg;
    OMG_Window** cache;
    bool (*init)(struct OMG_Winmgr* this);
    bool (*destroy)(struct OMG_Winmgr* this);
    OMG_Window* (*window_alloc)(struct OMG_Winmgr* this);
    bool (*window_free)(struct OMG_Winmgr* this, OMG_Window* window);
    OMG_Surface* (*surf_create)(struct OMG_Winmgr* this, const OMG_FPoint* size, bool has_alpha);
    bool (*surf_destroy)(struct OMG_Winmgr* this, OMG_Surface* surf);
    int surf_depth;
    bool surf_rle;
    bool was_allocated;
} OMG_Winmgr;

OMG_API bool omg_winmgr_init(OMG_Winmgr* this);
OMG_API bool omg_winmgr_destroy(OMG_Winmgr* this);
#if OMG_EXPORT_SHIT
OMG_API OMG_Surface* omg_winmgr_surf_create(OMG_Winmgr* this, const OMG_FPoint* size, bool has_alpha);
OMG_API bool omg_winmgr_surf_destroy(OMG_Winmgr* this, OMG_Surface* surf);
#endif
