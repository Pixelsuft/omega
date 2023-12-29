#pragma once
#include <omega/config.h>
#include <omega/ostd.h>
#include <omega/entry.h>
#include <omega/memory.h>
#include <omega/clock.h>
#include <omega/winmgr.h>
#include <omega/window.h>
#include <omega/events.h>

#define OMG_OMEGA_TYPE_NONE 0
#define OMG_OMEGA_TYPE_WIN 1
#define OMG_OMEGA_TYPE_SDL2 2
#define OMG_OMEGA_TYPE_RAYLIB 2

#define OMG_THEME_NONE 0
#define OMG_THEME_LIGHT 1
#define OMG_THEME_DARK 2
#define OMG_THEME_AUTO 3

#define OMG_LOG_CATEGORY_INFO 1
#define OMG_LOG_CATEGORY_WARN 2
#define OMG_LOG_CATEGORY_ERROR 3
#define OMG_LOG_CATEGORY_FATAL 4
#define OMG_LOG_CATEGORY_DISABLED 5

typedef struct OMG_Omega {
    OMG_Memory* mem;
    OMG_Std* std;
    OMG_EntryData* entry_data;
    OMG_Winmgr* winmgr;
    OMG_Clock* clock;
    OMG_Window** omg_window_cache;
    OMG_String* sdl2_dll_path;
    void* extra1;
    void* extra2;
    void* extra3;
    void* extra4;
    void* extra5;
    void* event_arg;
    bool (*omg_init)(struct OMG_Omega* this);
    bool (*app_init)(struct OMG_Omega* this);
    bool (*app_quit)(struct OMG_Omega* this);
    bool (*destroy)(struct OMG_Omega* this);
    bool (*winmgr_free)(struct OMG_Omega* this);
    bool (*winmgr_alloc)(struct OMG_Omega* this);
    void (*delay)(struct OMG_Omega* this, double seconds);
    void (*log_set_level)(struct OMG_Omega* this, const int log_level, const int omg_log_level, const int lib_log_level);
    bool (*log_info_str)(struct OMG_Omega* this, const OMG_String* data);
    bool (*log_warn_str)(struct OMG_Omega* this, const OMG_String* data);
    bool (*log_error_str)(struct OMG_Omega* this, const OMG_String* data);
    bool (*log_fatal_str)(struct OMG_Omega* this, const OMG_String* data);
    void (*auto_loop_run)(struct OMG_Omega* this);
    void (*auto_loop_stop)(struct OMG_Omega* this);
    OMG_Window* (*window_alloc)(struct OMG_Omega* this);
    bool (*window_free)(struct OMG_Omega* this, OMG_Window* window);
    void (*reset_event_handlers)(struct OMG_Omega* this);
    void (*on_quit)(OMG_EventQuit* event);
    void (*on_update)(OMG_EventUpdate* event);
    void (*on_paint)(OMG_EventPaint* event);
    void (*on_loop_stop)(OMG_EventLoopStop* event);
    OMG_FPoint scale;
    int log_level_lib;
    int log_level_omg;
    int log_level;
    int type;
    int theme;
    int app_theme;
    bool enable_paint;
    bool looping;
    bool support_highdpi;
    bool should_free_mem;
    bool should_free_std;
    bool inited;
} OMG_Omega;

OMG_API OMG_Omega* omg_create(OMG_EntryData* data);
OMG_API void omg_fill_on_create(OMG_Omega* this, OMG_EntryData* data);
OMG_API OMG_Omega* omg_get_default_omega(void);
OMG_API bool omg_destroy(OMG_Omega* this);
OMG_API bool omg_omg_init(OMG_Omega* this);
OMG_API void omg_reset_event_handlers(OMG_Omega* this);
OMG_API bool omg_alloc_winmgr(OMG_Omega* this);
OMG_API bool omg_app_init(OMG_Omega* this);
OMG_API bool omg_app_quit(OMG_Omega* this);
#if OMG_EXPORT_SHIT
// Should I always export events?
OMG_API void omg_event_on_quit(OMG_EventQuit* event);
OMG_API void omg_event_on_update(OMG_EventUpdate* event);
OMG_API void omg_event_on_paint(OMG_EventPaint* event);
OMG_API void omg_event_on_loop_stop(OMG_EventLoopStop* event);
OMG_API bool omg_free_winmgr(OMG_Omega* this);
OMG_API void omg_delay(OMG_Omega* this, double seconds);
OMG_API void omg_log_set_level(OMG_Omega* this, const int log_level, const int omg_log_level, const int lib_log_level);
OMG_API bool omg_log_info_str(OMG_Omega* this, const OMG_String* data);
OMG_API void omg_auto_loop_run(OMG_Omega* this);
OMG_API void omg_auto_loop_stop(OMG_Omega* this);
#endif
