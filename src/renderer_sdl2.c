#include <omega/renderer_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/window.h>
#include <omega/omega.h>
#define base ((OMG_Renderer*)this)
#define win_base ((OMG_Window*)base->win)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_renderer_sdl2_destroy(OMG_RendererSdl2* this) {
    if (base->inited) {
        this->sdl2->SDL_DestroyRenderer(this->ren);
        this->ren = NULL;
        base->inited = false;
    }
    return false;
}

bool omg_renderer_sdl2_clear(OMG_RendererSdl2* this, const OMG_Color* col) {
    bool res = false;
    if (this->sdl2->SDL_SetRenderDrawColor(this->ren, (uint8_t)col->r, (uint8_t)col->g, (uint8_t)col->b, (uint8_t)col->a) < 0) {
        res = true;
        _OMG_SDL2_DRAW_COLOR_WARN();
    }
    if (this->sdl2->SDL_RenderClear(this->ren) < 0) {
        res = true;
        _OMG_LOG_WARN(omg_base, "Failed to clear renderer (", this->sdl2->SDL_GetError(), ")");
    }
    return res;
}

bool omg_renderer_sdl2_flip(OMG_RendererSdl2* this) {
    this->sdl2->SDL_RenderPresent(this->ren);
    return false;
}

bool omg_renderer_sdl2_init(OMG_RendererSdl2* this) {
    OMG_BEGIN_POINTER_CAST();
    omg_renderer_init(this);
    base->destroy = omg_renderer_sdl2_destroy;
    base->clear = omg_renderer_sdl2_clear;
    base->flip = omg_renderer_sdl2_flip;
    OMG_END_POINTER_CAST();
    base->type = OMG_REN_TYPE_SDL2;
    // TODO: more customize
    this->ren = this->sdl2->SDL_CreateRenderer(
        this->win,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | (win_base->vsync ? SDL_RENDERER_PRESENTVSYNC : 0)
    );
    if (OMG_ISNULL(this->ren)) {
        _OMG_LOG_ERROR(omg_base, "Failed to create SDL2 renderer (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    base->inited = true;
    return false;
}
#endif
