#include <omega/api_sdl2_ttf.h>
#if OMG_SUPPORT_SDL2_TTF
#include <omega/ostd.h>
#if OMG_SDL2_TTF_DYNAMIC
#include <omega/api_static.h>
#define LOAD_REQUIRED(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#if OMG_SDL2_TTF_COMPAT_STATIC
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = NULL
#else
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = func_name
#endif
#endif

bool omg_sdl2_ttf_dll_load(OMG_Sdl2Ttf* this, const OMG_String* dll_path) {
#if OMG_SDL2_TTF_DYNAMIC
    if (OMG_ISNULL(dll_path))
#if OMG_IS_WIN
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("SDL2_ttf.dll"), L"SDL2_ttf.dll");
#else
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("libSDL2_ttf.so"), NULL);
#endif
    else
        this->handle = omg_static_lib_load(dll_path, NULL);
    if (OMG_ISNULL(this->handle))
        return true;
#else
    OMG_UNUSED(dll_path);
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED(TTF_Linked_Version);
#if OMG_SDL2_TTF_DYNAMIC
    const SDL_version* mix_ver = this->TTF_Linked_Version();
    this->ver.major = mix_ver->major;
    this->ver.minor = mix_ver->minor;
    this->ver.patch = mix_ver->patch;
#else
    this->ver.major = SDL_TTF_MAJOR_VERSION;
    this->ver.minor = SDL_TTF_MINOR_VERSION;
    this->ver.patch = SDL_TTF_PATCHLEVEL;
#endif
    LOAD_REQUIRED(TTF_Linked_Version);
    LOAD_REQUIRED(TTF_ByteSwappedUNICODE);
    LOAD_REQUIRED(TTF_Init);
    LOAD_REQUIRED(TTF_OpenFont);
    LOAD_REQUIRED(TTF_OpenFontIndex);
    LOAD_REQUIRED(TTF_OpenFontRW);
    LOAD_REQUIRED(TTF_OpenFontIndexRW);
    LOAD_REQUIRED(TTF_GetFontStyle);
    LOAD_REQUIRED(TTF_SetFontStyle);
    LOAD_REQUIRED(TTF_GetFontOutline);
    LOAD_REQUIRED(TTF_SetFontOutline);
    LOAD_REQUIRED(TTF_GetFontHinting);
    LOAD_REQUIRED(TTF_SetFontHinting);
    LOAD_REQUIRED(TTF_FontHeight);
    LOAD_REQUIRED(TTF_FontAscent);
    LOAD_REQUIRED(TTF_FontDescent);
    LOAD_REQUIRED(TTF_FontLineSkip);
    LOAD_REQUIRED(TTF_GetFontKerning);
    LOAD_REQUIRED(TTF_SetFontKerning);
    LOAD_REQUIRED(TTF_FontFaces);
    LOAD_REQUIRED(TTF_FontFaceIsFixedWidth);
    LOAD_REQUIRED(TTF_FontFaceFamilyName);
    LOAD_REQUIRED(TTF_FontFaceStyleName);
    LOAD_REQUIRED(TTF_GlyphIsProvided);
    LOAD_REQUIRED(TTF_GlyphMetrics);
    LOAD_REQUIRED(TTF_SizeText);
    LOAD_REQUIRED(TTF_SizeUTF8);
    LOAD_REQUIRED(TTF_SizeUNICODE);
    LOAD_REQUIRED(TTF_RenderText_Solid);
    LOAD_REQUIRED(TTF_RenderUTF8_Solid);
    LOAD_REQUIRED(TTF_RenderUNICODE_Solid);
    LOAD_REQUIRED(TTF_RenderGlyph_Solid);
    LOAD_REQUIRED(TTF_RenderText_Shaded);
    LOAD_REQUIRED(TTF_RenderUTF8_Shaded);
    LOAD_REQUIRED(TTF_RenderUNICODE_Shaded);
    LOAD_REQUIRED(TTF_RenderGlyph_Shaded);
    LOAD_REQUIRED(TTF_RenderText_Blended);
    LOAD_REQUIRED(TTF_RenderUTF8_Blended);
    LOAD_REQUIRED(TTF_RenderUNICODE_Blended);
    LOAD_REQUIRED(TTF_RenderGlyph_Blended);
    LOAD_REQUIRED(TTF_CloseFont);
    LOAD_REQUIRED(TTF_Quit);
    LOAD_REQUIRED(TTF_WasInit);
    LOAD_REQUIRED_COMPAT(TTF_GetFontKerningSizeGlyphs); // 2.0.14
    LOAD_REQUIRED_COMPAT(TTF_OpenFontDPI); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_OpenFontIndexDPI); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_OpenFontDPIRW); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_OpenFontIndexDPIRW); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_SetFontSize); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_SetFontSizeDPI); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_GlyphIsProvided32); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_GlyphMetrics32); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_MeasureText); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_MeasureUTF8); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_MeasureUNICODE); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_RenderText_Solid_Wrapped); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_RenderUTF8_Solid_Wrapped); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_RenderUNICODE_Solid_Wrapped); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_RenderGlyph32_Solid); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_RenderText_Shaded_Wrapped); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_RenderUTF8_Shaded_Wrapped); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_RenderUNICODE_Shaded_Wrapped); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_RenderGlyph32_Shaded); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_RenderText_Blended_Wrapped); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_RenderUTF8_Blended_Wrapped); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_RenderUNICODE_Blended_Wrapped); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_GetFontKerningSizeGlyphs32); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_RenderGlyph32_Blended); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_SetFontSDF); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_GetFontSDF); // 2.0.18
    LOAD_REQUIRED_COMPAT(TTF_GetFontWrappedAlign); // 2.0.20
    LOAD_REQUIRED_COMPAT(TTF_SetFontWrappedAlign); // 2.0.20
    LOAD_REQUIRED_COMPAT(TTF_RenderText_LCD); // 2.0.20
    LOAD_REQUIRED_COMPAT(TTF_RenderUTF8_LCD); // 2.0.20
    LOAD_REQUIRED_COMPAT(TTF_RenderUNICODE_LCD); // 2.0.20
    LOAD_REQUIRED_COMPAT(TTF_RenderText_LCD_Wrapped); // 2.0.20
    LOAD_REQUIRED_COMPAT(TTF_RenderUTF8_LCD_Wrapped); // 2.0.20
    LOAD_REQUIRED_COMPAT(TTF_RenderUNICODE_LCD_Wrapped); // 2.0.20
    LOAD_REQUIRED_COMPAT(TTF_RenderGlyph_LCD); // 2.0.20
    LOAD_REQUIRED_COMPAT(TTF_RenderGlyph32_LCD); // 2.0.20
    LOAD_REQUIRED_COMPAT(TTF_SetFontDirection); // 2.0.20
    LOAD_REQUIRED_COMPAT(TTF_SetFontScriptName); // 2.0.20
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_sdl2_ttf_dll_free(OMG_Sdl2Ttf* this) {
#if OMG_SDL2_TTF_DYNAMIC
    return omg_static_lib_free(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}
#endif
