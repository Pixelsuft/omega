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
#define SDL_INIT_TIMER          0x00000001u
#define SDL_INIT_VIDEO          0x00000020u
#define SDL_INIT_EVENTS         0x00004000u

#define SDL_WINDOWPOS_CENTERED_MASK    0x2FFF0000u
#define SDL_WINDOWPOS_CENTERED_DISPLAY(X)  (SDL_WINDOWPOS_CENTERED_MASK | (X))
#define SDL_WINDOWPOS_CENTERED         SDL_WINDOWPOS_CENTERED_DISPLAY(0)
#define SDL_WINDOWPOS_ISCENTERED(X)    (((X)&0xFFFF0000) == SDL_WINDOWPOS_CENTERED_MASK)

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

typedef void SDL_Window;
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
    void* OMG_SDL2_STD_PREFIX (*SDL_malloc)(size_t);
    void* OMG_SDL2_STD_PREFIX (*SDL_realloc)(void*, size_t size);
    void OMG_SDL2_STD_PREFIX (*SDL_free)(void*);
    void OMG_SDL2_STD_PREFIX (*SDL_LogInfo)(int, const char*, ...);
    void OMG_SDL2_STD_PREFIX (*SDL_LogWarn)(int, const char*, ...);
    void OMG_SDL2_STD_PREFIX (*SDL_LogError)(int, const char*, ...);
    void OMG_SDL2_STD_PREFIX (*SDL_LogCritical)(int, const char*, ...);
    const char* OMG_SDL2_STD_PREFIX (*SDL_GetError)(void);
    SDL_Window* OMG_SDL2_STD_PREFIX (*SDL_CreateWindow)(const char*, int, int, int, int, uint32_t);
    void OMG_SDL2_STD_PREFIX (*SDL_DestroyWindow)(SDL_Window*);
    void OMG_SDL2_STD_PREFIX (*SDL_ShowWindow)(SDL_Window*);
    SDL_version ver;
} OMG_Sdl2;

OMG_API bool omg_sdl2_dll_load(OMG_Sdl2* this, const OMG_String* dll_path);
OMG_API bool omg_sdl2_dll_free(OMG_Sdl2* this);

#endif
