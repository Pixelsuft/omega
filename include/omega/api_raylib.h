#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_RAYLIB
#include <omega/entry.h>
#if !OMG_RAYLIB_DYNAMIC
#include <raylib.h>
#endif
#define OMG_RAYLIB_PREFIX

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

typedef struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

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
    void OMG_RAYLIB_PREFIX (*InitWindow)(int, int, const char*);
    void OMG_RAYLIB_PREFIX (*CloseWindow)(void);
    bool OMG_RAYLIB_PREFIX (*WindowShouldClose)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowReady)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowState)(unsigned int);
    void OMG_RAYLIB_PREFIX (*SetWindowState)(unsigned int);
    void OMG_RAYLIB_PREFIX (*SetWindowTitle)(const char*);
    void OMG_RAYLIB_PREFIX (*SetConfigFlags)(unsigned int);
    void OMG_RAYLIB_PREFIX (*ClearWindowState)(unsigned int);
    void OMG_RAYLIB_PREFIX (*EnableEventWaiting)(void);
    void OMG_RAYLIB_PREFIX (*DisableEventWaiting)(void);
    void OMG_RAYLIB_PREFIX (*TraceLog)(int, const char*, ...);
    void OMG_RAYLIB_PREFIX (*SetTraceLogLevel)(int);
    void* OMG_RAYLIB_PREFIX (*MemAlloc)(unsigned int);
    void* OMG_RAYLIB_PREFIX (*MemRealloc)(void*, unsigned int);
    void OMG_RAYLIB_PREFIX (*MemFree)(void*);
    void OMG_RAYLIB_PREFIX (*WaitTime)(double);
    void OMG_RAYLIB_PREFIX (*SetTargetFPS)(int);
    float OMG_RAYLIB_PREFIX (*GetFrameTime)(void);
    int OMG_RAYLIB_PREFIX (*GetFPS)(void);
    void OMG_RAYLIB_PREFIX (*SetTraceLogCallback)(TraceLogCallback);
    void OMG_RAYLIB_PREFIX (*MaximizeWindow)(void);
    void OMG_RAYLIB_PREFIX (*MinimizeWindow)(void);
    void OMG_RAYLIB_PREFIX (*RestoreWindow)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowResized)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowFullscreen)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowHidden)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowMinimized)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowMaximized)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowFocused)(void);
    void OMG_RAYLIB_PREFIX (*ToggleFullscreen)(void);
    void OMG_RAYLIB_PREFIX (*ToggleBorderlessWindowed)(void);
    int OMG_RAYLIB_PREFIX (*GetScreenWidth)(void);
    int OMG_RAYLIB_PREFIX (*GetScreenHeight)(void);
    int OMG_RAYLIB_PREFIX (*GetRenderWidth)(void);
    int OMG_RAYLIB_PREFIX (*GetRenderHeight)(void);
    void OMG_RAYLIB_PREFIX (*SetWindowSize)(int, int);
    void OMG_RAYLIB_PREFIX (*SetWindowFocused)(void);
    void OMG_RAYLIB_PREFIX (*BeginDrawing)(void);
    void OMG_RAYLIB_PREFIX (*EndDrawing)(void);
    void OMG_RAYLIB_PREFIX (*ClearBackground)(Color);
} OMG_Raylib;

OMG_API bool omg_raylib_dll_load(OMG_Raylib* this, const OMG_String* dll_path);
OMG_API bool omg_raylib_dll_free(OMG_Raylib* this);

#endif
