#pragma once
#include <omega/config.h>

#if OMG_SUPPORT_BMFONT
#include <omega/omega.h>
#include <omega/renderer.h>

typedef struct {
    OMG_Renderer* ren;
    OMG_Texture* page;
    size_t chars_count;
    int size;
    int line_height;
    int base;
    int scale_w;
    int scale_h;
    bool bold;
    bool italic;
    bool unicode;
    bool smooth;
    bool aa;
} OMG_Bmfont;

OMG_API bool omg_bmfont_init(OMG_Bmfont* this, OMG_Texture* page, OMG_Renderer* ren);
OMG_API bool omg_bmfont_destroy(OMG_Bmfont* this);
#endif
