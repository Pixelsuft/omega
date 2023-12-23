#pragma once
#include <omega/ostd.h>
#include <omega/window.h>

typedef struct OMG_Winmgr {
    void* omg;
    OMG_Window** cache;
    bool (*init)(struct OMG_Winmgr* this);
    bool (*destroy)(struct OMG_Winmgr* this);
    OMG_Window* (*window_alloc)(struct OMG_Winmgr* this);
    bool (*window_free)(struct OMG_Winmgr* this, OMG_Window* window);
    bool was_allocated;
} OMG_Winmgr;

OMG_API bool omg_winmgr_init(OMG_Winmgr* this);
OMG_API bool omg_winmgr_destroy(OMG_Winmgr* this);
