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

#define TTF_STYLE_NORMAL 0x00
#define TTF_STYLE_BOLD 0x01
#define TTF_STYLE_ITALIC 0x02
#define TTF_STYLE_UNDERLINE 0x04
#define TTF_STYLE_STRIKETHROUGH 0x08

#define TTF_HINTING_NORMAL 0
#define TTF_HINTING_LIGHT 1
#define TTF_HINTING_MONO 2
#define TTF_HINTING_NONE 3
#define TTF_HINTING_LIGHT_SUBPIXEL 4

#define TTF_WRAPPED_ALIGN_LEFT 0
#define TTF_WRAPPED_ALIGN_CENTER 1
#define TTF_WRAPPED_ALIGN_RIGHT 2

typedef enum {
    TTF_DIRECTION_LTR = 0,
    TTF_DIRECTION_RTL,
    TTF_DIRECTION_TTB,
    TTF_DIRECTION_BTT
} TTF_Direction;

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
    void OMG_SDL2_TTF_STD_PREFIX (*TTF_SetFontStyle)(TTF_Font*, int);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_GetFontOutline)(const TTF_Font*);
    void OMG_SDL2_TTF_STD_PREFIX (*TTF_SetFontOutline)(TTF_Font*, int);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_GetFontHinting)(const TTF_Font*);
    void OMG_SDL2_TTF_STD_PREFIX (*TTF_SetFontHinting)(TTF_Font*, int);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_GetFontWrappedAlign)(const TTF_Font*);
    void OMG_SDL2_TTF_STD_PREFIX (*TTF_SetFontWrappedAlign)(TTF_Font*, int);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_FontHeight)(const TTF_Font*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_FontAscent)(const TTF_Font*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_FontDescent)(const TTF_Font*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_FontLineSkip)(const TTF_Font*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_GetFontKerning)(const TTF_Font*);
    void OMG_SDL2_TTF_STD_PREFIX (*TTF_SetFontKerning)(TTF_Font*, int);
    long OMG_SDL2_TTF_STD_PREFIX (*TTF_FontFaces)(const TTF_Font*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_FontFaceIsFixedWidth)(const TTF_Font*);
    const char* OMG_SDL2_TTF_STD_PREFIX (*TTF_FontFaceFamilyName)(const TTF_Font*);
    const char* OMG_SDL2_TTF_STD_PREFIX (*TTF_FontFaceStyleName)(const TTF_Font*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_GlyphIsProvided)(TTF_Font*, uint16_t ch);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_GlyphIsProvided32)(TTF_Font*, uint32_t);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_GlyphMetrics)(TTF_Font*, uint16_t, int*, int*, int*, int*, int*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_GlyphMetrics32)(TTF_Font*, uint32_t, int*, int*, int*, int*, int*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_SizeText)(TTF_Font*, const char*, int*, int*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_SizeUTF8)(TTF_Font*, const char*, int*, int*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_SizeUNICODE)(TTF_Font*, const uint16_t*, int*, int*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_MeasureText)(TTF_Font*, const char*, int, int*, int*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_MeasureUTF8)(TTF_Font*, const char*, int, int*, int*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_MeasureUNICODE)(TTF_Font*, const uint16_t*, int, int*, int*);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderText_Solid)(TTF_Font*, const char*, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUTF8_Solid)(TTF_Font*, const char*, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUNICODE_Solid)(TTF_Font*, const uint16_t*, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderText_Solid_Wrapped)(TTF_Font*, const char*, SDL_Color, uint32_t);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUTF8_Solid_Wrapped)(TTF_Font*, const char*, SDL_Color, uint32_t);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUNICODE_Solid_Wrapped)(TTF_Font*, const uint16_t*, SDL_Color, uint32_t);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderGlyph_Solid)(TTF_Font*, uint16_t, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderGlyph32_Solid)(TTF_Font*, uint32_t, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderText_Shaded)(TTF_Font*, const char*, SDL_Color, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUTF8_Shaded)(TTF_Font*, const char*, SDL_Color, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUNICODE_Shaded)(TTF_Font*, const uint16_t*, SDL_Color, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderText_Shaded_Wrapped)(TTF_Font*, const char*, SDL_Color, SDL_Color, uint32_t);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUTF8_Shaded_Wrapped)(TTF_Font*, const char*, SDL_Color, SDL_Color, uint32_t);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUNICODE_Shaded_Wrapped)(TTF_Font*, const uint16_t*, SDL_Color, SDL_Color, uint32_t);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderGlyph_Shaded)(TTF_Font*, uint16_t, SDL_Color, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderGlyph32_Shaded)(TTF_Font*, uint32_t, SDL_Color, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderText_Blended)(TTF_Font*, const char*, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUTF8_Blended)(TTF_Font*, const char*, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUNICODE_Blended)(TTF_Font*, const uint16_t*, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderText_Blended_Wrapped)(TTF_Font*, const char*, SDL_Color, uint32_t);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUTF8_Blended_Wrapped)(TTF_Font*, const char*, SDL_Color, uint32_t);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUNICODE_Blended_Wrapped)(TTF_Font*, const uint16_t*, SDL_Color, uint32_t);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderGlyph_Blended)(TTF_Font*, uint16_t, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderGlyph32_Blended)(TTF_Font*, uint32_t, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderText_LCD)(TTF_Font*, const char*, SDL_Color, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUTF8_LCD)(TTF_Font*, const char*, SDL_Color, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUNICODE_LCD)(TTF_Font*, const uint16_t*, SDL_Color, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderText_LCD_Wrapped)(TTF_Font*, const char*, SDL_Color, SDL_Color, uint32_t);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUTF8_LCD_Wrapped)(TTF_Font*, const char*, SDL_Color, SDL_Color, uint32_t);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderUNICODE_LCD_Wrapped)(TTF_Font*, const uint16_t*, SDL_Color, SDL_Color, uint32_t);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderGlyph_LCD)(TTF_Font*, uint16_t, SDL_Color, SDL_Color);
    SDL_Surface* OMG_SDL2_TTF_STD_PREFIX (*TTF_RenderGlyph32_LCD)(TTF_Font*, uint32_t, SDL_Color, SDL_Color);
    void OMG_SDL2_TTF_STD_PREFIX (*TTF_CloseFont)(TTF_Font*);
    void OMG_SDL2_TTF_STD_PREFIX (*TTF_Quit)(void);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_WasInit)(void);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_GetFontKerningSizeGlyphs)(TTF_Font*, uint16_t, uint16_t);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_GetFontKerningSizeGlyphs32)(TTF_Font*, uint32_t, uint32_t);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_SetFontSDF)(TTF_Font*, SDL_bool);
    SDL_bool OMG_SDL2_TTF_STD_PREFIX (*TTF_GetFontSDF)(const TTF_Font*);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_SetFontDirection)(TTF_Font*, TTF_Direction);
    int OMG_SDL2_TTF_STD_PREFIX (*TTF_SetFontScriptName)(TTF_Font*, const char*);
    SDL_version ver;
} OMG_Sdl2Ttf;

OMG_API bool omg_sdl2_ttf_dll_load(OMG_Sdl2Ttf* this, const OMG_String* dll_path);
OMG_API bool omg_sdl2_ttf_dll_free(OMG_Sdl2Ttf* this);
#endif
