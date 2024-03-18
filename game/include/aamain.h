#pragma once
#include <omega/omega.h>
#include <omega/window.h>
#include <omega/renderer.h>
#include <omega/audio.h>
#include <omega/image.h>

typedef struct {
    OMG_Omega* omg;
    OMG_Window* win;
    OMG_Renderer* ren;
    OMG_ImageLoader* img;
    OMG_Audio* au;
    OMG_Clock* clock;
} App;

extern App* app;
