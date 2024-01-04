#pragma once
#include <omega/ostd.h>

typedef struct {
    void* data;
    OMG_FPoint size;
    bool has_alpha;
} OMG_Surface;
