#pragma once
#include <omega/omega.h>
#include <omega/window.h>
#include <omega/renderer.h>
#include <omega/audio.h>
#include <omega/image.h>
#include <omega/scene.h>
#include <loader.h>

#define IS_BACK_CODE(code) ((code == OMG_SCANCODE_ESCAPE) || (code == OMG_SCANCODE_AC_BACK))
#define ASSETS_DIR "assets"

typedef struct {
    Loader ld;
    OMG_String bp;
    OMG_FPoint sc;
    OMG_Omega* omg;
    OMG_Window* win;
    OMG_Renderer* ren;
    OMG_ImageLoader* img;
    OMG_Audio* au;
    OMG_Clock* clock;
    OMG_SceneMgr sm;
} App;

extern App* _app;

OMG_Surface* app_load_surf(App* this, const OMG_String* path);
OMG_Texture* app_load_texture(App* this, const OMG_String* path);
