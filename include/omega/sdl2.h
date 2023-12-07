#pragma once
#include <omega/config.h>
#if OMG_SUPPORTS_SDL2
#include <omega/entry.h>
#if !OMG_SDL2_DYNAMIC
#if OMG_IS_VC || OMG_IS_EMSCRIPTEN || OMG_IS_ANDROID
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#endif

#if OMG_SDL2_DYNAMIC
#define SDL_INIT_TIMER          0x00000001u
#define SDL_INIT_VIDEO          0x00000020u
#define SDL_INIT_EVENTS         0x00004000u

typedef enum
{
    SDL_LOG_CATEGORY_APPLICATION
} SDL_LogCategory;

typedef struct {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
} SDL_version;
#endif

typedef struct {
    void* handle;
    int (*SDL_Init)(uint32_t);
    void (*SDL_Quit)(void);
    void (*SDL_GetVersion)(SDL_version*);
    void* (*SDL_memset)(void*, int, size_t);
    void* (*SDL_memcpy)(void*, const void*, size_t);
    void* (*SDL_memmove)(void*, const void*, size_t);
    int (*SDL_memcmp)(const void*, const void*, size_t);
    size_t (*SDL_strlen)(const char*);
    size_t (*SDL_utf8strlen)(const char*);
    size_t (*SDL_utf8strnlen)(const char*, size_t);
    char* (*SDL_strrev)(char*);
    char* (*SDL_itoa)(int, char*, int);
    char* (*SDL_ltoa)(long, char*, int);
    char* (*SDL_uitoa)(unsigned int, char*, int);
    char* (*SDL_ultoa)(unsigned long, char*, int);
    char* (*SDL_lltoa)(int64_t, char*, int);
    char* (*SDL_ulltoa)(uint64_t, char*, int);
    size_t (*SDL_wcslen)(const wchar_t*);
    void* (*SDL_malloc)(size_t);
    void* (*SDL_realloc)(void*, size_t size);
    void (*SDL_free)(void*);
    void (*SDL_LogInfo)(int, const char*, ...);
    void (*SDL_LogWarn)(int, const char*, ...);
    void (*SDL_LogError)(int, const char*, ...);
    void (*SDL_LogCritical)(int, const char*, ...);
    SDL_version ver;
} OMG_Sdl2;

OMG_API bool omg_sdl2_dll_load(OMG_Sdl2* this, const OMG_String* dll_path);
OMG_API bool omg_sdl2_dll_free(OMG_Sdl2* this);

#endif
