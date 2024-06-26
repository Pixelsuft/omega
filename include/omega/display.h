#pragma once
#include <omega/ostd.h>

#define OMG_DISPLAY_ORIENTATION_UNKNOWN 0
#define OMG_DISPLAY_ORIENTATION_LANDSCAPE 1
#define OMG_DISPLAY_ORIENTATION_LANDSCAPE_FLIPPED 2
#define OMG_DISPLAY_ORIENTATION_PORTRAIT 3
#define OMG_DISPLAY_ORIENTATION_PORTRAIT_FLIPPED 4

typedef struct {
    OMG_FPoint size;
    float rate;
} OMG_VideoMode;
