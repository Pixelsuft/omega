#pragma once
#include <omega/ostd.h>
#include <omega/window.h>
#include <omega/surface.h>
#include <omega/image.h>
#include <omega/font.h>
#include <omega/display.h>

typedef struct OMG_Winmgr {
    void* omg;
    OMG_Window** cache;
    OMG_ImageLoader* img;
    OMG_FontMgr* fnt;
    void* _img_init_ptr;
    size_t sz_image_loader;
    bool (*init)(struct OMG_Winmgr* this);
    bool (*destroy)(struct OMG_Winmgr* this);
    OMG_Window* (*window_alloc)(struct OMG_Winmgr* this);
    bool (*window_free)(struct OMG_Winmgr* this, OMG_Window* window);
    OMG_Surface* (*surf_create)(struct OMG_Winmgr* this, OMG_Surface* surf, const OMG_FPoint* size, bool has_alpha);
    OMG_Surface* (*surf_from_fp)(struct OMG_Winmgr* this, OMG_Surface* surf, const OMG_String* path, int format);
    OMG_Surface* (*surf_from_mem)(struct OMG_Winmgr* this, OMG_Surface* surf, const void* mem, size_t size, int format);
    OMG_Surface* (*surf_from_file)(struct OMG_Winmgr* this, OMG_Surface* surf, const OMG_File* file, bool destroy_file, int format);
    bool (*surf_destroy)(struct OMG_Winmgr* this, OMG_Surface* surf);
    bool (*surf_set_locked)(struct OMG_Winmgr* this, OMG_Surface* surf, bool locked);
    bool (*image_loader_alloc)(struct OMG_Winmgr* this);
    bool (*image_loader_free)(struct OMG_Winmgr* this);
    bool (*fontmgr_alloc)(struct OMG_Winmgr* this);
    bool (*fontmgr_free)(struct OMG_Winmgr* this);
    int (*display_get_count)(struct OMG_Winmgr* this);
    OMG_String (*display_get_name)(struct OMG_Winmgr* this, int display_id);
    bool (*display_get_bounds)(struct OMG_Winmgr* this, int display_id, OMG_FRect* rect, bool only_usable);
    bool (*display_get_scale)(struct OMG_Winmgr* this, int display_id, OMG_FRect* scale);
    int (*display_get_num_modes)(struct OMG_Winmgr* this, int display_id);
    bool (*display_get_mode)(struct OMG_Winmgr* this, int display_id, int mode_id, OMG_VideoMode* mode);
    int surf_depth;
    int image_formats;
    int fnt_type;
    bool surf_rle;
    bool inited;
    bool was_allocated;
} OMG_Winmgr;

OMG_API bool omg_winmgr_init(OMG_Winmgr* this);
OMG_API bool omg_winmgr_destroy(OMG_Winmgr* this);
OMG_API OMG_Surface* omg_winmgr_surf_create(OMG_Winmgr* this, OMG_Surface* surf, const OMG_FPoint* size, bool has_alpha);
OMG_API OMG_Surface* omg_winmgr_dummy_surf_create(OMG_Winmgr* this);
OMG_API OMG_String omg_winmgr_display_get_name(OMG_Winmgr* this, int display_id);
OMG_API bool omg_winmgr_display_get_bounds(OMG_Winmgr* this, int display_id, OMG_FRect* rect, bool only_usable);
OMG_API bool omg_winmgr_display_get_scale(OMG_Winmgr* this, int display_id, OMG_FRect* scale);
#if OMG_EXPORT_SHIT
OMG_API bool omg_winmgr_surf_destroy(OMG_Winmgr* this, OMG_Surface* surf);
OMG_API bool omg_winmgr_surf_set_locked(OMG_Winmgr* this, OMG_Surface* surf, bool locked);
OMG_API bool omg_winmgr_image_loader_alloc(OMG_Winmgr* this);
OMG_API bool omg_winmgr_image_loader_free(OMG_Winmgr* this);
OMG_API bool omg_winmgr_fontmgr_alloc(OMG_Winmgr* this);
OMG_API bool omg_winmgr_fontmgr_free(OMG_Winmgr* this);
OMG_API bool omg_winmgr_display_get_mode(OMG_Winmgr* this, int display_id, int mode_id, OMG_VideoMode* mode);
OMG_API int omg_winmgr_display_get_count(OMG_Winmgr* this);
OMG_API int omg_winmgr_display_get_num_modes(OMG_Winmgr* this, int display_id);
OMG_API OMG_Surface* omg_winmgr_surf_from_fp(OMG_Winmgr* this, OMG_Surface* surf, const OMG_String* path, int format);
OMG_API OMG_Surface* omg_winmgr_surf_from_mem(OMG_Winmgr* this, OMG_Surface* surf, const void* mem, size_t size, int format);
OMG_API OMG_Surface* omg_winmgr_surf_from_file(OMG_Winmgr* this, OMG_Surface* surf, OMG_File* file, bool destroy_file, int format);
#endif
