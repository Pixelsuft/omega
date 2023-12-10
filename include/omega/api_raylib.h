#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_RAYLIB
#include <omega/entry.h>
#if !OMG_RAYLIB_DYNAMIC
#include <raylib.h>
#endif

#if OMG_RAYLIB_DYNAMIC
typedef enum {
    FLAG_VSYNC_HINT         = 0x00000040,
    FLAG_FULLSCREEN_MODE    = 0x00000002,
    FLAG_WINDOW_RESIZABLE   = 0x00000004,
    FLAG_WINDOW_UNDECORATED = 0x00000008,
    FLAG_WINDOW_HIDDEN      = 0x00000080,
    FLAG_WINDOW_MINIMIZED   = 0x00000200,
    FLAG_WINDOW_MAXIMIZED   = 0x00000400,
    FLAG_WINDOW_UNFOCUSED   = 0x00000800,
    FLAG_WINDOW_TOPMOST     = 0x00001000,
    FLAG_WINDOW_ALWAYS_RUN  = 0x00000100,
    FLAG_WINDOW_TRANSPARENT = 0x00000010,
    FLAG_WINDOW_HIGHDPI     = 0x00002000,
    FLAG_WINDOW_MOUSE_PASSTHROUGH = 0x00004000,
    FLAG_BORDERLESS_WINDOWED_MODE = 0x00008000,
    FLAG_MSAA_4X_HINT       = 0x00000020,
    FLAG_INTERLACED_HINT    = 0x00010000
} ConfigFlags;

typedef enum {
    LOG_ALL = 0,
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
    LOG_NONE
} TraceLogLevel;

typedef void (*TraceLogCallback)(int logLevel, const char *text, va_list args);
#endif

typedef struct {
    void* handle;
    void (*InitWindow)(int, int, const char*);
    void (*CloseWindow)(void);
    bool (*WindowShouldClose)(void);
    bool (*IsWindowState)(unsigned int);
    void (*SetWindowState)(unsigned int);
    void (*ClearWindowState)(unsigned int);
    void (*EnableEventWaiting)(void);
    void (*DisableEventWaiting)(void);
    void (*TraceLog)(int, const char*, ...);
    void (*SetTraceLogLevel)(int);
    void* (*MemAlloc)(unsigned int);
    void* (*MemRealloc)(void*, unsigned int);
    void (*MemFree)(void*);
    void (*SetTraceLogCallback)(TraceLogCallback);
} OMG_Raylib;

OMG_API bool omg_raylib_dll_load(OMG_Raylib* this, const OMG_String* dll_path);
OMG_API bool omg_raylib_dll_free(OMG_Raylib* this);

#endif
