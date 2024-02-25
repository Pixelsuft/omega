#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_SCENES
#include <omega/omega.h>
#include <omega/renderer.h>

typedef struct {
    OMG_Renderer* ren;
    OMG_Omega* omg;
} OMG_SceneMgr;
#endif
