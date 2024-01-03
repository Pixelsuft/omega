#include <omega/api_sdl2.h>
#if OMG_SUPPORT_SDL2
#include <omega/ostd.h>
#if OMG_SDL2_DYNAMIC
#include <omega/api_static.h>
#define LOAD_REQUIRED(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#if OMG_SDL2_COMOMG_SDL2_COMPAT_STATIC
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = NULL
#else
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = func_name
#endif
#endif

static OMG_Sdl2* omg_sdl2_cache = NULL;

int omg_sdl2_render_copyf_emu(SDL_Renderer* ren, SDL_Texture* tex, const SDL_Rect* src, const SDL_FRect* dst) {
    if (OMG_ISNULL(dst))
        return omg_sdl2_cache->SDL_RenderCopy(ren, tex, src, NULL);
    SDL_Rect dst_i;
    dst_i.x = (int)dst->x;
    dst_i.y = (int)dst->y;
    dst_i.w = (int)dst->w;
    dst_i.h = (int)dst->h;
    return omg_sdl2_cache->SDL_RenderCopy(ren, tex, src, &dst_i);
}

int omg_sdl2_render_copy_exf_emu(SDL_Renderer* ren, SDL_Texture* tex, const SDL_Rect* src, const SDL_FRect* dst, const double angle, const SDL_FPoint* center, const SDL_RendererFlip flip) {
    SDL_Rect dst_i;
    if (OMG_ISNOTNULL(dst)) {
        dst_i.x = (int)dst->x;
        dst_i.y = (int)dst->y;
        dst_i.w = (int)dst->w;
        dst_i.h = (int)dst->h;
    }
    SDL_Point center_i;
    if (OMG_ISNOTNULL(center)) {
        center_i.x = (int)center->x;
        center_i.y = (int)center->y;
    }
    return omg_sdl2_cache->SDL_RenderCopyEx(ren, tex, src, OMG_ISNULL(dst) ? NULL : &dst_i, angle, OMG_ISNULL(center) ? NULL : &center_i, flip);
}

int omg_sdl2_render_draw_pointf_emu(SDL_Renderer* ren, float px, float py) {
    return omg_sdl2_cache->SDL_RenderDrawPoint(ren, (int)px, (int)py);
}

int omg_sdl2_render_draw_rectf_emu(SDL_Renderer* ren, const SDL_FRect* rect) {
    if (OMG_ISNULL(rect))
        return omg_sdl2_cache->SDL_RenderDrawRect(ren, NULL);
    SDL_Rect rect_i;
    rect_i.x = (int)rect->x;
    rect_i.y = (int)rect->y;
    rect_i.w = (int)rect->w;
    rect_i.h = (int)rect->h;
    return omg_sdl2_cache->SDL_RenderDrawRect(ren, &rect_i);
}

int omg_sdl2_render_fill_rectf_emu(SDL_Renderer* ren, const SDL_FRect* rect) {
    if (OMG_ISNULL(rect))
        return omg_sdl2_cache->SDL_RenderFillRect(ren, NULL);
    SDL_Rect rect_i;
    rect_i.x = (int)rect->x;
    rect_i.y = (int)rect->y;
    rect_i.w = (int)rect->w;
    rect_i.h = (int)rect->h;
    return omg_sdl2_cache->SDL_RenderFillRect(ren, &rect_i);
}

uint64_t omg_sdl2_get_ticks64_emu(void) {
    return (uint64_t)omg_sdl2_cache->SDL_GetTicks();
}

static bool omg_sdl2_already_loaded = false;

bool omg_sdl2_dll_load(OMG_Sdl2* this, const OMG_String* dll_path) {
#if OMG_SDL2_DYNAMIC
    if (OMG_ISNULL(dll_path))
#if OMG_IS_WIN
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("SDL2.dll"), L"SDL2.dll");
#else
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("libSDL2.so"), NULL);
#endif
    else
        this->handle = omg_static_lib_load(dll_path, NULL);
    if (OMG_ISNULL(this->handle))
        return true;
#else
    OMG_UNUSED(dll_path);
    SDL_VERSION(&this->ver);
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED(SDL_GetVersion);
#if OMG_SDL2_DYNAMIC
    this->SDL_GetVersion(&this->ver);
#endif
    LOAD_REQUIRED(SDL_Init);
    LOAD_REQUIRED(SDL_Quit);
    LOAD_REQUIRED(SDL_Delay);
    LOAD_REQUIRED(SDL_memset);
    LOAD_REQUIRED(SDL_memcpy);
    LOAD_REQUIRED(SDL_memmove);
    LOAD_REQUIRED(SDL_memcmp);
    LOAD_REQUIRED(SDL_strcmp);
    LOAD_REQUIRED(SDL_strlen);
    LOAD_REQUIRED(SDL_utf8strlen);
    LOAD_REQUIRED(SDL_utf8strnlen);
    LOAD_REQUIRED(SDL_strrev);
    LOAD_REQUIRED(SDL_itoa);
    LOAD_REQUIRED(SDL_ltoa);
    LOAD_REQUIRED(SDL_uitoa);
    LOAD_REQUIRED(SDL_ultoa);
    LOAD_REQUIRED(SDL_lltoa);
    LOAD_REQUIRED(SDL_ulltoa);
    LOAD_REQUIRED(SDL_wcslen);
    LOAD_REQUIRED(SDL_floor);
    LOAD_REQUIRED(SDL_ceil);
    LOAD_REQUIRED(SDL_round);
    LOAD_REQUIRED(SDL_floorf);
    LOAD_REQUIRED(SDL_ceilf);
    LOAD_REQUIRED(SDL_roundf);
    LOAD_REQUIRED(SDL_malloc);
    LOAD_REQUIRED(SDL_realloc);
    LOAD_REQUIRED(SDL_free);
    LOAD_REQUIRED(SDL_LogInfo);
    LOAD_REQUIRED(SDL_LogWarn);
    LOAD_REQUIRED(SDL_LogError);
    LOAD_REQUIRED(SDL_LogCritical);
    LOAD_REQUIRED(SDL_GetError);
    LOAD_REQUIRED(SDL_GetMouseFocus);
    LOAD_REQUIRED(SDL_GetMouseState);
    LOAD_REQUIRED(SDL_GetRelativeMouseState);
    LOAD_REQUIRED(SDL_CreateWindow);
    LOAD_REQUIRED(SDL_CreateWindowFrom);
    LOAD_REQUIRED(SDL_DestroyWindow);
    LOAD_REQUIRED(SDL_GetWindowSize);
    LOAD_REQUIRED(SDL_GetWindowID);
    LOAD_REQUIRED(SDL_ShowWindow);
    LOAD_REQUIRED(SDL_HideWindow);
    LOAD_REQUIRED(SDL_SetWindowFullscreen);
    LOAD_REQUIRED(SDL_SetWindowBordered);
    LOAD_REQUIRED(SDL_SetWindowSize);
    LOAD_REQUIRED(SDL_SetWindowMaximumSize);
    LOAD_REQUIRED(SDL_SetWindowMinimumSize);
    LOAD_REQUIRED(SDL_MinimizeWindow);
    LOAD_REQUIRED(SDL_MaximizeWindow);
    LOAD_REQUIRED(SDL_RestoreWindow);
    LOAD_REQUIRED(SDL_SetWindowTitle);
    LOAD_REQUIRED(SDL_PollEvent);
    LOAD_REQUIRED(SDL_StartTextInput);
    LOAD_REQUIRED(SDL_StopTextInput);
    LOAD_REQUIRED(SDL_IsTextInputActive);
    LOAD_REQUIRED(SDL_IsScreenKeyboardShown);
    LOAD_REQUIRED(SDL_HasScreenKeyboardSupport);
    LOAD_REQUIRED(SDL_SetTextInputRect);
    LOAD_REQUIRED(SDL_GetTicks);
    LOAD_REQUIRED(SDL_GetPerformanceFrequency);
    LOAD_REQUIRED(SDL_GetPerformanceCounter);
    LOAD_REQUIRED(SDL_GetNumRenderDrivers);
    LOAD_REQUIRED(SDL_GetRenderDriverInfo);
    LOAD_REQUIRED(SDL_GetRendererInfo);
    LOAD_REQUIRED(SDL_CreateRenderer);
    LOAD_REQUIRED(SDL_GetRendererOutputSize);
    LOAD_REQUIRED(SDL_DestroyRenderer);
    LOAD_REQUIRED(SDL_CreateRGBSurfaceWithFormat);
    LOAD_REQUIRED(SDL_CreateRGBSurfaceWithFormatFrom);
    LOAD_REQUIRED(SDL_CreateRGBSurfaceFrom);
    LOAD_REQUIRED(SDL_FreeSurface);
    LOAD_REQUIRED(SDL_CreateTexture);
    LOAD_REQUIRED(SDL_DestroyTexture);
    LOAD_REQUIRED(SDL_QueryTexture);
    LOAD_REQUIRED(SDL_SetTextureColorMod);
    LOAD_REQUIRED(SDL_SetTextureAlphaMod);
    LOAD_REQUIRED(SDL_SetTextureBlendMode);
    LOAD_REQUIRED(SDL_SetTextureScaleMode);
    LOAD_REQUIRED(SDL_UpdateTexture);
    LOAD_REQUIRED(SDL_LockTexture);
    LOAD_REQUIRED(SDL_UnlockTexture);
    LOAD_REQUIRED(SDL_RenderSetViewport);
    LOAD_REQUIRED(SDL_RenderSetClipRect);
    LOAD_REQUIRED(SDL_SetRenderDrawBlendMode);
    LOAD_REQUIRED(SDL_RenderSetScale);
    LOAD_REQUIRED(SDL_RenderSetVSync);
    LOAD_REQUIRED(SDL_RenderClear);
    LOAD_REQUIRED(SDL_RenderCopy);
    LOAD_REQUIRED(SDL_RenderCopyEx);
    LOAD_REQUIRED(SDL_RenderDrawPoint);
    LOAD_REQUIRED(SDL_RenderDrawPoints);
    LOAD_REQUIRED(SDL_RenderDrawRect);
    LOAD_REQUIRED(SDL_RenderFillRect);
    LOAD_REQUIRED(SDL_SetRenderTarget);
    LOAD_REQUIRED(SDL_SetRenderDrawColor);
    LOAD_REQUIRED(SDL_RenderPresent);
    // TODO: check using SDL2 versions
    LOAD_REQUIRED_COMPAT(SDL_SetWindowResizable); // 2.0.5
    LOAD_REQUIRED_COMPAT(SDL_GetNumAllocations); // 2.0.7
    LOAD_REQUIRED_COMPAT(SDL_RenderCopyF); // 2.0.10
    LOAD_REQUIRED_COMPAT(SDL_RenderCopyExF); // 2.0.10
    LOAD_REQUIRED_COMPAT(SDL_RenderDrawPointF); // 2.0.10
    LOAD_REQUIRED_COMPAT(SDL_RenderDrawPointsF); // 2.0.10
    LOAD_REQUIRED_COMPAT(SDL_RenderDrawRectF); // 2.0.10
    LOAD_REQUIRED_COMPAT(SDL_RenderFillRectF); // 2.0.10
    LOAD_REQUIRED_COMPAT(SDL_SetWindowAlwaysOnTop); // 2.0.16
    LOAD_REQUIRED_COMPAT(SDL_RenderGeometryRaw); // 2.0.18
    LOAD_REQUIRED_COMPAT(SDL_RenderGeometry); // 2.0.18
    LOAD_REQUIRED_COMPAT(SDL_GetTicks64); // 2.0.18
    if (OMG_ISNULL(this->SDL_RenderCopyF))
        this->SDL_RenderCopyF = omg_sdl2_render_copyf_emu;
    if (OMG_ISNULL(this->SDL_RenderCopyExF))
        this->SDL_RenderCopyExF = omg_sdl2_render_copy_exf_emu;
    if (OMG_ISNULL(this->SDL_RenderDrawPointF))
        this->SDL_RenderDrawPointF = omg_sdl2_render_draw_pointf_emu;
    if (OMG_ISNULL(this->SDL_RenderDrawRectF))
        this->SDL_RenderDrawRectF = omg_sdl2_render_draw_rectf_emu;
    if (OMG_ISNULL(this->SDL_RenderFillRectF))
        this->SDL_RenderFillRectF = omg_sdl2_render_fill_rectf_emu;
    if (OMG_ISNULL(this->SDL_GetTicks64))
        this->SDL_GetTicks64 = omg_sdl2_get_ticks64_emu;
    OMG_END_POINTER_CAST();
    omg_sdl2_cache = this;
    if (omg_sdl2_already_loaded) {
        this->is_first = false;
    }
    else {
        omg_sdl2_already_loaded = true;
        this->is_first = true;
    }
    return false;
}

bool omg_sdl2_dll_free(OMG_Sdl2* this) {
    if (!this->is_first)
        return false;
    omg_sdl2_already_loaded = false;
#if OMG_SDL2_DYNAMIC
    return omg_static_lib_free(this->handle);
#else
    return false;
#endif
}

#endif
