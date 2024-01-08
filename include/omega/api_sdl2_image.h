#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_SDL2_IMAGE
#include <omega/entry.h>
#include <omega/api_sdl2.h>
#if !OMG_SDL2_DYNAMIC
#if OMG_IS_VC || OMG_IS_UNIX || OMG_IS_EMSCRIPTEN || OMG_IS_ANDROID
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif
#endif

#if OMG_SDL2_DYNAMIC
#define OMG_SDL2_IMAGE_STD_PREFIX

typedef enum {
    IMG_INIT_JPG    = 0x00000001,
    IMG_INIT_PNG    = 0x00000002,
    IMG_INIT_TIF    = 0x00000004,
    IMG_INIT_WEBP   = 0x00000008,
    IMG_INIT_JXL    = 0x00000010,
    IMG_INIT_AVIF   = 0x00000020
} IMG_InitFlags;
#else
#define OMG_SDL2_IMAGE_STD_PREFIX SDLCALL
#endif

typedef struct {
    void* handle;
    int OMG_SDL2_IMAGE_STD_PREFIX (*IMG_Init)(int);
    void OMG_SDL2_IMAGE_STD_PREFIX (*IMG_Quit)(void);
    SDL_Surface* OMG_SDL2_IMAGE_STD_PREFIX IMG_LoadTyped_RW(SDL_RWops*, int, const char*);
    SDL_Surface* OMG_SDL2_IMAGE_STD_PREFIX (*IMG_Load)(const char*);
} OMG_Sdl2Image;
#endif
