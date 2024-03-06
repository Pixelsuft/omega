#pragma once
#include <omega/config.h>

#if OMG_SUPPORT_BMFONT
#include <omega/omega.h>
#include <omega/renderer.h>
#include <omega/array.h>

typedef struct {
    int x;
    int y;
    int w;
    int h;
    int xo;
    int yo;
    int xa;
} OMG_Bmchar;

typedef struct {
    OMG_Renderer* ren;
    OMG_Omega* omg;
    OMG_Array(OMG_Bmchar) chars;
    OMG_Texture* page;
    size_t ch_count;
    size_t k_count;
    int pad[4];
    int spac[2];
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

// Init a bitmap font
OMG_API bool omg_bmfont_init(OMG_Bmfont* this, OMG_Texture* page, OMG_Renderer* ren, char* data, size_t data_len);
// Destroy a bitmap font
OMG_API bool omg_bmfont_destroy(OMG_Bmfont* this);
// Render a line
OMG_API bool omg_bmfont_render(OMG_Bmfont* this, const OMG_String* text, const OMG_FPoint* pos);
#endif
