#include <omega/window_win.h>
#include <omega/winmgr_win.h>
#include <omega/omega.h>

#if OMG_SUPPORT_WIN
#define base ((OMG_Winmgr*)this)
#define surf_base ((OMG_Surface*)surf)
#define omg_base ((OMG_Omega*)base->omg)
#include <omega/surface_win.h>

OMG_WindowWin* omg_winmgr_win_window_alloc(OMG_WinmgrWin* this) {
    OMG_WindowWin* result = OMG_MALLOC(omg_base->mem, sizeof(OMG_WindowWin));
    if (OMG_ISNULL(result)) {
        _OMG_LOG_ERROR(omg_base, "Failed to allocate memory for Win32 Window");
        return NULL;
    }
    OMG_BEGIN_POINTER_CAST();
    omg_window_fill_on_create(result);
    result->parent.omg = omg_base;
    result->u32 = this->u32;
    result->k32 = this->k32;
    result->dwm = this->dwm;
    result->g32 = this->g32;
    result->win_build_num = this->win_build_num;
    result->uxtheme = this->uxtheme;
    result->parent.was_allocated = true;
    result->parent.default_init = omg_window_win_init;
    OMG_END_POINTER_CAST();
    return result;
}

bool omg_winmgr_win_window_free(OMG_WinmgrWin* this, OMG_WindowWin* window) {
    if (OMG_ISNULL(window))
        return true;
    ((OMG_Window*)window)->destroy((OMG_Window*)window);
    if (((OMG_Window*)window)->was_allocated)
        return OMG_FREE(omg_base->mem, window);
    return false;
}

bool omg_winmgr_win_destroy(OMG_WinmgrWin* this) {
    omg_winmgr_destroy((OMG_Winmgr*)this);
#if OMG_SUPPORT_SDL2
    if (OMG_ISNOTNULL(this->sdl2)) {
        if (this->sdl2->SDL_WasInit(0) > 0)
            this->sdl2->SDL_Quit();
        omg_sdl2_dll_free(this->sdl2);
        OMG_FREE(omg_base->mem, this->sdl2);
        this->sdl2 = NULL;
    }
#endif
    return false;
}

bool omg_winmgr_win_surf_destroy(OMG_WinmgrWin* this, OMG_SurfaceWin* surf) {
    if (OMG_ISNULL(surf)) {
        _OMG_NULL_SURFACE_WARN();
        return true;
    }
    bool res = false;
    if (OMG_ISNULL(surf_base->data)) {
        if (OMG_ISNOTNULL(surf->bm) && !this->g32->DeleteObject(surf->bm)) {
            res = true;
            _OMG_LOG_WARN(omg_base, "Failed to delete win32 bitmap");
        }
        if (OMG_ISNOTNULL(surf->dc) && !this->g32->DeleteDC(surf->dc)) {
            res = true;
            _OMG_LOG_WARN(omg_base, "Failed to delete Win32 DC");
        }
    }
    else {
        OMG_FREE(omg_base->mem, surf_base->data);
        surf_base->data = NULL;
    }
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    if (surf_base->was_allocated)
#endif
    OMG_FREE(omg_base->mem, surf);
    return res;
}

OMG_SurfaceWin* omg_winmgr_win_surf_from_fp(OMG_WinmgrWin* this, OMG_SurfaceWin* surf, const OMG_String* path, int format) {
    if (OMG_ISNULL(surf)) {
        surf = OMG_MALLOC(omg_base->mem, sizeof(OMG_SurfaceWin));
        if (OMG_ISNULL(surf))
            return (OMG_SurfaceWin*)omg_winmgr_dummy_surf_create(base);
#if OMG_ALLOW_SURF_WAS_ALLOCATED
        surf_base->was_allocated = true;
#endif
    }
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    else
        surf_base->was_allocated = false;
#endif
#if OMG_SUPPORT_OMG_IMAGE
    if (base->img->type == OMG_IMAGE_LOADER_TYPE_OMG) {
        struct {
            void* data;
            int w, h, depth;
        } img_buf;
        if (base->img->image_from_internal(base->img, 0, path, &img_buf, format)) {
            omg_winmgr_win_surf_destroy(this, surf);
            return (OMG_SurfaceWin*)omg_winmgr_dummy_surf_create(base);
        }
        surf_base->data = img_buf.data;
        surf_base->size.w = (float)img_buf.w;
        surf_base->size.h = (float)img_buf.h;
        surf_base->has_alpha = true;
    }
#endif
    return surf;
}

OMG_SurfaceWin* omg_winmgr_win_surf_from_mem(OMG_WinmgrWin* this, OMG_SurfaceWin* surf, const void* mem, size_t size, int format) {
    if (OMG_ISNULL(surf)) {
        surf = OMG_MALLOC(omg_base->mem, sizeof(OMG_SurfaceWin));
        if (OMG_ISNULL(surf))
            return (OMG_SurfaceWin*)omg_winmgr_dummy_surf_create(base);
#if OMG_ALLOW_SURF_WAS_ALLOCATED
        surf_base->was_allocated = true;
#endif
    }
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    else
        surf_base->was_allocated = false;
#endif
#if OMG_SUPPORT_OMG_IMAGE
    if (base->img->type == OMG_IMAGE_LOADER_TYPE_OMG) {
        struct {
            void* data;
            int w, h, depth;
        } img_buf;
        OMG_DataWithSize data = { .data = mem, .size = size };
        if (base->img->image_from_internal(base->img, 1, &data, &img_buf, format)) {
            omg_winmgr_win_surf_destroy(this, surf);
            return (OMG_SurfaceWin*)omg_winmgr_dummy_surf_create(base);
        }
        surf_base->data = img_buf.data;
        surf_base->size.w = (float)img_buf.w;
        surf_base->size.h = (float)img_buf.h;
        surf_base->has_alpha = true;
    }
#endif
    return surf;
}

OMG_SurfaceWin* omg_winmgr_win_surf_create(OMG_WinmgrWin* this, OMG_SurfaceWin* surf, const OMG_FPoint* size, bool has_alpha) {
    if (OMG_ISNULL(surf)) {
        surf = OMG_MALLOC(omg_base->mem, sizeof(OMG_SurfaceWin));
        if (OMG_ISNULL(surf))
            return (OMG_SurfaceWin*)omg_winmgr_dummy_surf_create(base);
#if OMG_ALLOW_SURF_WAS_ALLOCATED
        surf_base->was_allocated = true;
#endif
    }
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    else
        surf_base->was_allocated = false;
#endif
    surf->dc = this->g32->CreateCompatibleDC(NULL);
    if (OMG_ISNULL(surf->dc)) {
        OMG_FREE(omg_base->mem, surf);
        _OMG_LOG_ERROR(omg_base, "Failed to create Win32 HDC for surface");
        return (OMG_SurfaceWin*)omg_winmgr_dummy_surf_create(base);
    }
    surf->bm = this->g32->CreateCompatibleBitmap(surf->dc, (int)size->w, (int)size->h);
    if (OMG_ISNULL(surf->bm)) {
        this->g32->DeleteDC(surf->dc);
        OMG_FREE(omg_base->mem, surf);
        _OMG_LOG_ERROR(omg_base, "Failed to create Win32 bitmap for surface");
        return (OMG_SurfaceWin*)omg_winmgr_dummy_surf_create(base);
    }
    surf_base->data = NULL;
    surf_base->size.w = size->w;
    surf_base->size.h = size->h;
    surf_base->has_alpha = has_alpha;
    return surf;
}

int omg_winmgr_win_display_get_count(OMG_WinmgrWin* this) {
    int counter = 0;
    DISPLAY_DEVICEW dev_d, mon_d;
    dev_d.cb = mon_d.cb = sizeof(DISPLAY_DEVICEW);
    DWORD dev_index = 0;
    while (this->u32->EnumDisplayDevicesW(NULL, dev_index, &dev_d, 0)) {
        DWORD mon_index = 0;
        while (this->u32->EnumDisplayDevicesW(dev_d.DeviceName, mon_index, &mon_d, 0)) {
            mon_index++;
            counter++;
        }
        dev_index++;
    }
    return counter;
}

bool omg_winmgr_win_find_display(OMG_WinmgrWin* this, DISPLAY_DEVICEW* dev_d, DISPLAY_DEVICEW* monitor_dev, int display_id) {
    dev_d->cb = monitor_dev->cb = sizeof(DISPLAY_DEVICEW);
    int counter = 0;
    DWORD dev_index = 0;
    while (this->u32->EnumDisplayDevicesW(NULL, dev_index, dev_d, 0)) {
        DWORD mon_index = 0;
        while (this->u32->EnumDisplayDevicesW(dev_d->DeviceName, mon_index, monitor_dev, 0)) {
            if (counter >= display_id)
                return false;
            mon_index++;
            counter++;
        }
        dev_index++;
    }
    return true;
}

OMG_String omg_winmgr_win_display_get_name(OMG_WinmgrWin* this, int display_id) {
    DISPLAY_DEVICEW mon_d, dev_d;
    if (omg_winmgr_win_find_display(this, &dev_d, &mon_d, display_id))
        return omg_winmgr_display_get_name(base, display_id);
    static char monitor_name_buf[sizeof(mon_d.DeviceString) * 2];
    OMG_String res = OMG_STRING_MAKE_BUFFER_A(monitor_name_buf);
    res.len = 0;
    if (omg_string_add_wchar_p(&res, mon_d.DeviceString))
        return omg_winmgr_display_get_name(base, display_id);
    for (size_t i = 0; i < res.len; i++) {
        if (res.ptr[i] == ',') {
            res.ptr[i] = '\0';
            res.len = i;
            break;
        }
    }
    return res;
}

BOOL omg_winmgr_win_monitor_check_enum(HMONITOR hmon, HDC hdc, LPRECT mon_rect, LPARAM lparam) {
    OMG_UNUSED(hmon, hdc);
    OMG_WinmgrWin* this = (OMG_WinmgrWin*)lparam;
    if (this->mon_counter == this->need_display) {
        this->temp_rect.x = (float)mon_rect->left;
        this->temp_rect.y = (float)mon_rect->top;
        this->temp_rect.w = (float)(mon_rect->right - mon_rect->left);
        this->temp_rect.h = (float)(mon_rect->bottom - mon_rect->top);
        return TRUE;
    }
    this->mon_counter++;
    return TRUE;
}

bool omg_winmgr_win_display_get_bounds(OMG_WinmgrWin* this, int display_id, OMG_FRect* rect, bool only_usable) {
    this->mon_counter = 0;
    this->need_display = display_id;
    if (!this->u32->EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)omg_winmgr_win_monitor_check_enum, (LPARAM)this))
        return omg_winmgr_display_get_bounds(base, display_id, rect, only_usable);
    rect->x = this->temp_rect.x;
    rect->y = this->temp_rect.y;
    rect->w = this->temp_rect.w;
    rect->h = this->temp_rect.h;
    return false;
}

bool omg_winmgr_win_display_get_current_mode(OMG_WinmgrWin* this, int display_id, OMG_VideoMode* mode) {
    DISPLAY_DEVICEW dev_d, mon_d;
    DEVMODEW dev_mode_w;
    dev_mode_w.dmSize = sizeof(DEVMODEW);
    if (omg_winmgr_win_find_display(this, &dev_d, &mon_d, display_id))
        return omg_winmgr_display_get_current_mode(base, display_id, mode);
    if (!this->u32->EnumDisplaySettingsW(dev_d.DeviceName, ENUM_CURRENT_SETTINGS, &dev_mode_w)) {
        _OMG_LOG_WARN(omg_base, "Failed to get current display settings", (int)this->k32->GetLastError());
        return true;
    }
    mode->size.w = (float)dev_mode_w.dmPelsWidth;
    mode->size.h = (float)dev_mode_w.dmPelsHeight;
    mode->rate = (float)dev_mode_w.dmDisplayFrequency;
    return false;
}

bool omg_winmgr_win_display_get_scale(OMG_WinmgrWin* this, int display_id, OMG_FRect* scale) {
    // TODO https://stackoverflow.com/questions/70976583/get-real-screen-resolution-using-win32-api
    return omg_winmgr_display_get_scale(base, display_id, scale);
}

bool omg_winmgr_win_init(OMG_WinmgrWin* this) {
    if (omg_winmgr_init((OMG_Winmgr*)this))
        return true;
    base->fnt_type = OMG_FONT_MGR_SDL2;
#if OMG_SUPPORT_SDL2
    this->sdl2 = NULL;
#endif
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_winmgr_win_destroy;
    base->window_alloc = omg_winmgr_win_window_alloc;
    base->window_free = omg_winmgr_win_window_free;
    base->surf_create = omg_winmgr_win_surf_create;
    base->surf_destroy = omg_winmgr_win_surf_destroy;
    base->surf_from_fp = omg_winmgr_win_surf_from_fp;
    base->surf_from_mem = omg_winmgr_win_surf_from_mem;
    base->display_get_count = omg_winmgr_win_display_get_count;
    base->display_get_name = omg_winmgr_win_display_get_name;
    base->display_get_bounds = omg_winmgr_win_display_get_bounds;
    base->display_get_scale = omg_winmgr_win_display_get_scale;
    base->display_get_current_mode = omg_winmgr_win_display_get_current_mode;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
