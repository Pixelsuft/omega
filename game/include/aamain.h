#pragma once
#include <omega/omega.h>
#include <omega/window.h>
#include <omega/renderer.h>
#include <omega/audio.h>
#include <omega/image.h>
#include <omega/scene.h>

#define IS_BACK_CODE(code) ((code == OMG_SCANCODE_ESCAPE) || (code == OMG_SCANCODE_AC_BACK))

typedef struct {
    OMG_String bp;
    OMG_Omega* omg;
    OMG_Window* win;
    OMG_Renderer* ren;
    OMG_ImageLoader* img;
    OMG_Audio* au;
    OMG_Clock* clock;
    OMG_SceneMgr sm;
} App;

extern App* _app;
