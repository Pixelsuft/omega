#pragma once
#include <omega/ostd.h>

#define OMG_IMAGE_LOADER_TYPE_NONE 0
#define OMG_IMAGE_LOADER_TYPE_SDL2 1

typedef struct {
    int type;
} OMG_ImageLoader;
