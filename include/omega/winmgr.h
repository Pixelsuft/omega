#pragma once
#include <omega/ostd.h>
#include <omega/window.h>
#include <omega/surface.h>
#include <omega/image.h>

typedef struct OMG_Winmgr {
    void* omg;
    OMG_Window** cache;
    OMG_ImageLoader* img;
    void* _img_init_ptr;
    size_t sz_image_loader;
    bool (*init)(struct OMG_Winmgr* this);
    bool (*destroy)(struct OMG_Winmgr* this);
    OMG_Window* (*window_alloc)(struct OMG_Winmgr* this);
    bool (*window_free)(struct OMG_Winmgr* this, OMG_Window* window);
    OMG_Surface* (*surf_create)(struct OMG_Winmgr* this, const OMG_FPoint* size, bool has_alpha);
    OMG_Surface* (*surf_from_path)(struct OMG_Winmgr* this, const OMG_String* path);
    bool (*surf_destroy)(struct OMG_Winmgr* this, OMG_Surface* surf);
    bool (*image_loader_alloc)(struct OMG_Winmgr* this);
    bool (*image_loader_free)(struct OMG_Winmgr* this);
    int surf_depth;
    int image_formats;
    bool surf_rle;
    bool inited;
    bool was_allocated;
} OMG_Winmgr;

OMG_API bool omg_winmgr_init(OMG_Winmgr* this);
OMG_API bool omg_winmgr_destroy(OMG_Winmgr* this);
OMG_API OMG_Surface* omg_winmgr_surf_create(OMG_Winmgr* this, const OMG_FPoint* size, bool has_alpha);
OMG_API OMG_Surface* omg_winmgr_dummy_surf_create(OMG_Winmgr* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_winmgr_surf_destroy(OMG_Winmgr* this, OMG_Surface* surf);
OMG_API bool omg_winmgr_image_loader_alloc(OMG_Winmgr* this);
OMG_API bool omg_winmgr_image_loader_free(OMG_Winmgr* this);
OMG_API OMG_Surface* omg_winmgr_surf_from_path(OMG_Winmgr* this, const OMG_String* path);
#endif
