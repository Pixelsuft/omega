#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_SDL2
#include <omega/entry.h>
#if !OMG_SDL2_DYNAMIC
#if OMG_IS_VC || OMG_IS_UNIX || OMG_IS_EMSCRIPTEN || OMG_IS_ANDROID
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#endif

#if OMG_SDL2_DYNAMIC
#define OMG_SDL2_STD_PREFIX

#ifdef __CC_ARM
#define SDL_FALSE 0
#define SDL_TRUE 1
typedef int SDL_bool;
#else
typedef enum
{
    SDL_FALSE = 0,
    SDL_TRUE = 1
} SDL_bool;
#endif

#define SDL_INIT_TIMER          0x00000001u
#define SDL_INIT_VIDEO          0x00000020u
#define SDL_INIT_EVENTS         0x00004000u

#define SDL_WINDOWPOS_UNDEFINED_MASK    0x1FFF0000u
#define SDL_WINDOWPOS_UNDEFINED_DISPLAY(X)  (SDL_WINDOWPOS_UNDEFINED_MASK | (X))
#define SDL_WINDOWPOS_UNDEFINED         SDL_WINDOWPOS_UNDEFINED_DISPLAY(0)
#define SDL_WINDOWPOS_ISUNDEFINED(X)    (((X) & 0xFFFF0000) == SDL_WINDOWPOS_UNDEFINED_MASK)

#define SDL_WINDOWPOS_CENTERED_MASK    0x2FFF0000u
#define SDL_WINDOWPOS_CENTERED_DISPLAY(X)  (SDL_WINDOWPOS_CENTERED_MASK | (X))
#define SDL_WINDOWPOS_CENTERED         SDL_WINDOWPOS_CENTERED_DISPLAY(0)
#define SDL_WINDOWPOS_ISCENTERED(X)    (((X) & 0xFFFF0000) == SDL_WINDOWPOS_CENTERED_MASK)

typedef enum
{
    SDL_LOG_CATEGORY_APPLICATION
} SDL_LogCategory;

typedef struct {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
} SDL_version;

typedef enum
{
    SDL_WINDOWEVENT_NONE,
    SDL_WINDOWEVENT_SHOWN,
    SDL_WINDOWEVENT_HIDDEN,
    SDL_WINDOWEVENT_EXPOSED,
    SDL_WINDOWEVENT_MOVED,
    SDL_WINDOWEVENT_RESIZED,
    SDL_WINDOWEVENT_SIZE_CHANGED,
    SDL_WINDOWEVENT_MINIMIZED,
    SDL_WINDOWEVENT_MAXIMIZED,
    SDL_WINDOWEVENT_RESTORED,
    SDL_WINDOWEVENT_ENTER,
    SDL_WINDOWEVENT_LEAVE,
    SDL_WINDOWEVENT_FOCUS_GAINED,
    SDL_WINDOWEVENT_FOCUS_LOST,
    SDL_WINDOWEVENT_CLOSE,
    SDL_WINDOWEVENT_TAKE_FOCUS,
    SDL_WINDOWEVENT_HIT_TEST,
    SDL_WINDOWEVENT_ICCPROF_CHANGED,
    SDL_WINDOWEVENT_DISPLAY_CHANGED
} SDL_WindowEventID;

typedef enum
{
    SDL_FIRSTEVENT = 0,
    SDL_QUIT = 0x100,
    SDL_APP_TERMINATING,
    SDL_APP_LOWMEMORY,
    SDL_APP_WILLENTERBACKGROUND,
    SDL_APP_DIDENTERBACKGROUND,
    SDL_APP_WILLENTERFOREGROUND,
    SDL_APP_DIDENTERFOREGROUND,
    SDL_LOCALECHANGED,
    SDL_DISPLAYEVENT = 0x150,
    SDL_WINDOWEVENT = 0x200,
    SDL_SYSWMEVENT,
    SDL_KEYDOWN = 0x300,
    SDL_KEYUP,
    SDL_TEXTEDITING,
    SDL_TEXTINPUT,
    SDL_KEYMAPCHANGED,
    SDL_TEXTEDITING_EXT,
    SDL_MOUSEMOTION = 0x400,
    SDL_MOUSEBUTTONDOWN,
    SDL_MOUSEBUTTONUP,
    SDL_MOUSEWHEEL,
    SDL_JOYAXISMOTION = 0x600,
    SDL_JOYBALLMOTION,
    SDL_JOYHATMOTION,
    SDL_JOYBUTTONDOWN,
    SDL_JOYBUTTONUP,
    SDL_JOYDEVICEADDED,
    SDL_JOYDEVICEREMOVED,
    SDL_JOYBATTERYUPDATED,
    SDL_CONTROLLERAXISMOTION = 0x650,
    SDL_CONTROLLERBUTTONDOWN,
    SDL_CONTROLLERBUTTONUP,
    SDL_CONTROLLERDEVICEADDED,
    SDL_CONTROLLERDEVICEREMOVED,
    SDL_CONTROLLERDEVICEREMAPPED,
    SDL_CONTROLLERTOUCHPADDOWN,
    SDL_CONTROLLERTOUCHPADMOTION,
    SDL_CONTROLLERTOUCHPADUP,
    SDL_CONTROLLERSENSORUPDATE,
    SDL_FINGERDOWN = 0x700,
    SDL_FINGERUP,
    SDL_FINGERMOTION,
    SDL_DOLLARGESTURE = 0x800,
    SDL_DOLLARRECORD,
    SDL_MULTIGESTURE,
    SDL_CLIPBOARDUPDATE = 0x900,
    SDL_DROPFILE = 0x1000,
    SDL_DROPTEXT,
    SDL_DROPBEGIN,
    SDL_DROPCOMPLETE,
    SDL_AUDIODEVICEADDED = 0x1100,
    SDL_AUDIODEVICEREMOVED,
    SDL_SENSORUPDATE = 0x1200,
    SDL_RENDER_TARGETS_RESET = 0x2000,
    SDL_RENDER_DEVICE_RESET,
    SDL_POLLSENTINEL = 0x7F00,
    SDL_USEREVENT = 0x8000,
    SDL_LASTEVENT = 0xFFFF
} SDL_EventType;

typedef struct SDL_CommonEvent
{
    uint32_t type;
    uint32_t timestamp;
} SDL_CommonEvent;

typedef struct SDL_WindowEvent
{
    uint32_t type;
    uint32_t timestamp;
    uint32_t windowID;
    uint8_t event;
    uint8_t padding1;
    uint8_t padding2;
    uint8_t padding3;
    int32_t data1;
    int32_t data2;
} SDL_WindowEvent;

typedef union SDL_Event
{
    uint32_t type;
    SDL_CommonEvent common;
    SDL_WindowEvent window;
    uint8_t padding[sizeof(void*) <= 8 ? 56 : sizeof(void*) == 16 ? 64 : 3 * sizeof(void*)];
} SDL_Event;

typedef enum
{
    SDL_WINDOW_FULLSCREEN = 0x00000001,
    SDL_WINDOW_OPENGL = 0x00000002,
    SDL_WINDOW_SHOWN = 0x00000004,
    SDL_WINDOW_HIDDEN = 0x00000008,
    SDL_WINDOW_BORDERLESS = 0x00000010,
    SDL_WINDOW_RESIZABLE = 0x00000020,
    SDL_WINDOW_MINIMIZED = 0x00000040,
    SDL_WINDOW_MAXIMIZED = 0x00000080,
    SDL_WINDOW_MOUSE_GRABBED = 0x00000100,
    SDL_WINDOW_INPUT_FOCUS = 0x00000200,
    SDL_WINDOW_MOUSE_FOCUS = 0x00000400,
    SDL_WINDOW_FULLSCREEN_DESKTOP = (SDL_WINDOW_FULLSCREEN | 0x00001000),
    SDL_WINDOW_FOREIGN = 0x00000800,
    SDL_WINDOW_ALLOW_HIGHDPI = 0x00002000,
    SDL_WINDOW_MOUSE_CAPTURE = 0x00004000,
    SDL_WINDOW_ALWAYS_ON_TOP = 0x00008000,
    SDL_WINDOW_SKIP_TASKBAR = 0x00010000,
    SDL_WINDOW_UTILITY = 0x00020000,
    SDL_WINDOW_TOOLTIP = 0x00040000,
    SDL_WINDOW_POPUP_MENU = 0x00080000,
    SDL_WINDOW_KEYBOARD_GRABBED = 0x00100000,
    SDL_WINDOW_VULKAN = 0x10000000,
    SDL_WINDOW_METAL = 0x20000000,
    SDL_WINDOW_INPUT_GRABBED = SDL_WINDOW_MOUSE_GRABBED
} SDL_WindowFlags;

typedef enum
{
    SDL_RENDERER_SOFTWARE = 0x00000001,
    SDL_RENDERER_ACCELERATED = 0x00000002,
    SDL_RENDERER_PRESENTVSYNC = 0x00000004,
    SDL_RENDERER_TARGETTEXTURE = 0x00000008
} SDL_RendererFlags;

typedef struct SDL_RendererInfo
{
    const char* name;
    uint32_t flags;
    uint32_t num_texture_formats;
    uint32_t texture_formats[16];
    int max_texture_width;
    int max_texture_height;
} SDL_RendererInfo;

typedef void SDL_Window;
typedef void SDL_Renderer;
#else
#define OMG_SDL2_STD_PREFIX SDLCALL
#endif

typedef struct {
    void* handle;
    int OMG_SDL2_STD_PREFIX (*SDL_Init)(uint32_t);
    void OMG_SDL2_STD_PREFIX (*SDL_Quit)(void);
    void OMG_SDL2_STD_PREFIX (*SDL_GetVersion)(SDL_version*);
    void OMG_SDL2_STD_PREFIX (*SDL_Delay)(uint32_t);
    void* OMG_SDL2_STD_PREFIX (*SDL_memset)(void*, int, size_t);
    void* OMG_SDL2_STD_PREFIX (*SDL_memcpy)(void*, const void*, size_t);
    void* OMG_SDL2_STD_PREFIX (*SDL_memmove)(void*, const void*, size_t);
    int OMG_SDL2_STD_PREFIX (*SDL_memcmp)(const void*, const void*, size_t);
    int OMG_SDL2_STD_PREFIX (*SDL_strcmp)(const char*, const char*);
    size_t OMG_SDL2_STD_PREFIX (*SDL_strlen)(const char*);
    size_t OMG_SDL2_STD_PREFIX (*SDL_utf8strlen)(const char*);
    size_t OMG_SDL2_STD_PREFIX (*SDL_utf8strnlen)(const char*, size_t);
    char* OMG_SDL2_STD_PREFIX (*SDL_strrev)(char*);
    char* OMG_SDL2_STD_PREFIX (*SDL_itoa)(int, char*, int);
    char* OMG_SDL2_STD_PREFIX (*SDL_ltoa)(long, char*, int);
    char* OMG_SDL2_STD_PREFIX (*SDL_uitoa)(unsigned int, char*, int);
    char* OMG_SDL2_STD_PREFIX (*SDL_ultoa)(unsigned long, char*, int);
    char* OMG_SDL2_STD_PREFIX (*SDL_lltoa)(int64_t, char*, int);
    char* OMG_SDL2_STD_PREFIX (*SDL_ulltoa)(uint64_t, char*, int);
    size_t OMG_SDL2_STD_PREFIX (*SDL_wcslen)(const wchar_t*);
    double OMG_SDL2_STD_PREFIX (*SDL_floor)(double);
    double OMG_SDL2_STD_PREFIX (*SDL_ceil)(double);
    double OMG_SDL2_STD_PREFIX (*SDL_round)(double);
    float OMG_SDL2_STD_PREFIX (*SDL_floorf)(float);
    float OMG_SDL2_STD_PREFIX (*SDL_ceilf)(float);
    float OMG_SDL2_STD_PREFIX (*SDL_roundf)(float);
    int OMG_SDL2_STD_PREFIX (*SDL_GetNumAllocations)(void);
    void* OMG_SDL2_STD_PREFIX (*SDL_malloc)(size_t);
    void* OMG_SDL2_STD_PREFIX (*SDL_realloc)(void*, size_t);
    void OMG_SDL2_STD_PREFIX (*SDL_free)(void*);
    void OMG_SDL2_STD_PREFIX (*SDL_LogInfo)(int, const char*, ...);
    void OMG_SDL2_STD_PREFIX (*SDL_LogWarn)(int, const char*, ...);
    void OMG_SDL2_STD_PREFIX (*SDL_LogError)(int, const char*, ...);
    void OMG_SDL2_STD_PREFIX (*SDL_LogCritical)(int, const char*, ...);
    const char* OMG_SDL2_STD_PREFIX (*SDL_GetError)(void);
    SDL_Window* OMG_SDL2_STD_PREFIX (*SDL_CreateWindow)(const char*, int, int, int, int, uint32_t);
    SDL_Window* OMG_SDL2_STD_PREFIX (*SDL_CreateWindowFrom)(const void*);
    void OMG_SDL2_STD_PREFIX (*SDL_DestroyWindow)(SDL_Window*);
    void OMG_SDL2_STD_PREFIX (*SDL_GetWindowSize)(SDL_Window*, int*, int*);
    uint32_t OMG_SDL2_STD_PREFIX (*SDL_GetWindowID)(SDL_Window*);
    void OMG_SDL2_STD_PREFIX (*SDL_ShowWindow)(SDL_Window*);
    void OMG_SDL2_STD_PREFIX (*SDL_HideWindow)(SDL_Window*);
    void OMG_SDL2_STD_PREFIX (*SDL_SetWindowResizable)(SDL_Window*, SDL_bool);
    void OMG_SDL2_STD_PREFIX (*SDL_SetWindowBordered)(SDL_Window*, SDL_bool);
    void OMG_SDL2_STD_PREFIX (*SDL_SetWindowAlwaysOnTop)(SDL_Window*, SDL_bool);
    void OMG_SDL2_STD_PREFIX (*SDL_MinimizeWindow)(SDL_Window*);
    void OMG_SDL2_STD_PREFIX (*SDL_MaximizeWindow)(SDL_Window*);
    void OMG_SDL2_STD_PREFIX (*SDL_RestoreWindow)(SDL_Window*);
    void OMG_SDL2_STD_PREFIX (*SDL_SetWindowTitle)(SDL_Window*, const char*);
    int OMG_SDL2_STD_PREFIX (*SDL_PollEvent)(SDL_Event*);
    uint32_t OMG_SDL2_STD_PREFIX (*SDL_GetTicks)(void);
    uint64_t OMG_SDL2_STD_PREFIX (*SDL_GetTicks64)(void);
    uint64_t OMG_SDL2_STD_PREFIX (*SDL_GetPerformanceFrequency)(void);
    uint64_t OMG_SDL2_STD_PREFIX (*SDL_GetPerformanceCounter)(void);
    int OMG_SDL2_STD_PREFIX (*SDL_GetNumRenderDrivers)(void);
    int OMG_SDL2_STD_PREFIX (*SDL_GetRenderDriverInfo)(int, SDL_RendererInfo*);
    int OMG_SDL2_STD_PREFIX (*SDL_GetRendererInfo)(SDL_Renderer*, SDL_RendererInfo*);
    SDL_Renderer* OMG_SDL2_STD_PREFIX (*SDL_CreateRenderer)(SDL_Window*, int, uint32_t);
    int OMG_SDL2_STD_PREFIX (*SDL_GetRendererOutputSize)(SDL_Renderer*, int*, int*);
    void OMG_SDL2_STD_PREFIX (*SDL_DestroyRenderer)(SDL_Renderer*);
    int OMG_SDL2_STD_PREFIX (*SDL_RenderClear)(SDL_Renderer*);
    int OMG_SDL2_STD_PREFIX (*SDL_SetRenderDrawColor)(SDL_Renderer*, uint8_t, uint8_t, uint8_t, uint8_t);
    void OMG_SDL2_STD_PREFIX (*SDL_RenderPresent)(SDL_Renderer*);
    SDL_version ver;
} OMG_Sdl2;

OMG_API bool omg_sdl2_dll_load(OMG_Sdl2* this, const OMG_String* dll_path);
OMG_API bool omg_sdl2_dll_free(OMG_Sdl2* this);
#if OMG_EXPORT_SHIT
OMG_API uint64_t omg_sdl2_get_ticks64_emu(void);
#endif
#endif
