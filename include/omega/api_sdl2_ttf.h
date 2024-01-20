#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_SDL2_TTF
#include <omega/api_sdl2.h>
#if !OMG_SDL2_TTF_DYNAMIC
#if OMG_IS_VC || OMG_IS_UNIX || OMG_IS_EMSCRIPTEN || OMG_IS_ANDROID
#include <SDL_ttf.h>
#else
#include <SDL2/SDL_ttf.h>
#endif
#endif

#if OMG_SDL2_TTF_DYNAMIC
#define OMG_SDL2_TTF_STD_PREFIX

#define UNICODE_BOM_NATIVE  0xFEFF
#define UNICODE_BOM_SWAPPED 0xFFFE

#define TTF_STYLE_NORMAL        0x00
#define TTF_STYLE_BOLD          0x01
#define TTF_STYLE_ITALIC        0x02
#define TTF_STYLE_UNDERLINE     0x04
#define TTF_STYLE_STRIKETHROUGH 0x08

typedef void TTF_Font;
#else
#define OMG_SDL2_TTF_STD_PREFIX SDLCALL
#endif

typedef struct {
    void* handle;
    const SDL_version* OMG_SDL2_TTF_STD_PREFIX (*TTF_Linked_Version)(void);
    void OMG_SDL2_TTF_STD_PREFIX (*TTF_ByteSwappedUNICODE)(SDL_bool);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_Init)(void);
    TTF_Font* OMG_SDL2_TTF_STD_PREFIX (*TTF_OpenFont)(const char*, int);
    TTF_Font* OMG_SDL2_TTF_STD_PREFIX (*TTF_OpenFontIndex)(const char*, int, long);
    TTF_Font* OMG_SDL2_TTF_STD_PREFIX (*TTF_OpenFontRW)(SDL_RWops*, int, int);
    TTF_Font* OMG_SDL2_TTF_STD_PREFIX (*TTF_OpenFontIndexRW)(SDL_RWops*, int, int, long);
    TTF_Font* OMG_SDL2_TTF_STD_PREFIX (*TTF_OpenFontDPI)(const char*, int, unsigned int, unsigned int);
    TTF_Font* OMG_SDL2_TTF_STD_PREFIX (*TTF_OpenFontIndexDPI)(const char*, int, long, unsigned int, unsigned int);
    TTF_Font* OMG_SDL2_TTF_STD_PREFIX (*TTF_OpenFontDPIRW)(SDL_RWops*, int, int, unsigned int, unsigned int);
    TTF_Font* OMG_SDL2_TTF_STD_PREFIX (*TTF_OpenFontIndexDPIRW)(SDL_RWops*, int, int, long, unsigned int, unsigned int);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_SetFontSize)(TTF_Font*, int);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_SetFontSizeDPI)(TTF_Font*, int, unsigned int, unsigned int);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_GetFontStyle)(const TTF_Font*);
    SDL_version ver;
} OMG_Sdl2Ttf;

OMG_API bool omg_sdl2_ttf_dll_load(OMG_Sdl2Ttf* this, const OMG_String* dll_path);
OMG_API bool omg_sdl2_ttf_dll_free(OMG_Sdl2Ttf* this);
#endif
