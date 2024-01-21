#include <omega/font_sdl2.h>

#if OMG_SUPPORT_SDL2_TTF
#include <omega/omega.h>
#include <omega/omega_sdl2.h>

#define base ((OMG_FontMgr*)this)
#define omg_base ((OMG_Omega*)base->omg)
#define TTF_GETERROR() ((omg_base->type == OMG_OMEGA_TYPE_SDL2) ? ((OMG_OmegaSdl2*)omg_base)->sdl2->SDL_GetError() : "")

bool omg_fontmgr_sdl2_destroy(OMG_FontMgrSdl2* this) {
    if (!base->inited)
        return false;
    base->inited = false;
    this->ttf.TTF_Quit();
    return omg_sdl2_ttf_dll_free(&this->ttf);
}

bool omg_fontmgr_sdl2_init(OMG_FontMgrSdl2* this) {
    omg_fontmgr_init(base);
    if (omg_sdl2_ttf_dll_load(&this->ttf, omg_base->sdl2_ttf_dll_path)) {
        _OMG_LOG_ERROR(omg_base, "Failed to load SDL2_ttf dll");
        return true;
    }
    if (this->ttf.TTF_Init() < 0) {
        _OMG_LOG_ERROR(omg_base, "Failed to load init SDL2_ttf (", TTF_GETERROR(), ")");
        omg_sdl2_ttf_dll_free(&this->ttf);
        return true;
    }
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_fontmgr_sdl2_destroy;
    OMG_END_POINTER_CAST();
    base->inited = true;
    return false;
}
#endif
