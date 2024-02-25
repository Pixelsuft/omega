#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_SCENES
#include <omega/ostd.h>

typedef struct {
    void* omg_ren;
    void* omg_omg;
} OMG_SceneMgr;

OMG_API bool omg_scenemgr_init(OMG_SceneMgr* this, void* omg_ren);
OMG_API bool omg_scenemgr_destroy(OMG_SceneMgr* this);
#endif
