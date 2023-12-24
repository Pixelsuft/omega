#pragma once
#include <omega/ostd.h>

#define OMG_REN_TYPE_NONE 0
#define OMG_REN_TYPE_AUTO 1
#define OMG_REN_TYPE_WIN 2
#define OMG_REN_TYPE_SDL2 3

typedef struct {
    void* omg;
    void* window;
    int type;
    bool inited;
    bool was_allocated;
} OMG_Renderer;
