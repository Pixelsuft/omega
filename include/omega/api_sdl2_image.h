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

typedef struct {
	int w, h;
	int count;
	SDL_Surface** frames;
	int* delays;
} IMG_Animation;
#else
#define OMG_SDL2_IMAGE_STD_PREFIX SDLCALL
#endif

typedef struct {
    void* handle;
    int OMG_SDL2_IMAGE_STD_PREFIX (*IMG_Init)(int);
    void OMG_SDL2_IMAGE_STD_PREFIX (*IMG_Quit)(void);
    SDL_Surface* OMG_SDL2_IMAGE_STD_PREFIX (*IMG_LoadTyped_RW)(SDL_RWops*, int, const char*);
    SDL_Surface* OMG_SDL2_IMAGE_STD_PREFIX (*IMG_Load)(const char*);
    SDL_Surface* OMG_SDL2_IMAGE_STD_PREFIX (*IMG_Load_RW)(SDL_RWops*, int);
    SDL_Texture* OMG_SDL2_IMAGE_STD_PREFIX (*IMG_LoadTexture)(SDL_Renderer*, const char*);
    SDL_Texture* OMG_SDL2_IMAGE_STD_PREFIX (*IMG_LoadTexture_RW)(SDL_Renderer*, SDL_RWops*, int);
    SDL_Texture* OMG_SDL2_IMAGE_STD_PREFIX (*IMG_LoadTextureTyped_RW)(SDL_Renderer*, SDL_RWops*, int, const char*);
    int OMG_SDL2_IMAGE_STD_PREFIX (*IMG_SavePNG)(SDL_Surface*, const char*);
    int OMG_SDL2_IMAGE_STD_PREFIX (*IMG_SavePNG_RW)(SDL_Surface*, SDL_RWops*, int);
    int OMG_SDL2_IMAGE_STD_PREFIX (*IMG_SaveJPG)(SDL_Surface*, const char*);
    int OMG_SDL2_IMAGE_STD_PREFIX (*IMG_SaveJPG_RW)(SDL_Surface*, SDL_RWops*, int);
    bool is_first;
} OMG_Sdl2Image;

OMG_API bool omg_sdl2_image_dll_load(OMG_Sdl2Image* this, const OMG_String* dll_path);
OMG_API bool omg_sdl2_image_dll_free(OMG_Sdl2Image* this);
#endif
