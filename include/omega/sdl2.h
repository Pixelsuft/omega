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
    SDL_version ver;
} OMG_Sdl2;

OMG_API bool omg_sdl2_dll_load(OMG_Sdl2* this, const OMG_String* dll_path);
OMG_API bool omg_sdl2_dll_free(OMG_Sdl2* this);

#endif
