#include <omega/renderer_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/window.h>
#include <omega/omega.h>
#define base ((OMG_Renderer*)this)
#define win_base ((OMG_Window*)base->win)
#define omg_base ((OMG_Omega*)base->omg)

void omg_renderer_sdl2_update_scale(OMG_RendererSdl2* this) {
    if (!omg_base->support_highdpi)
        return;
    int win_w, win_h, ren_w, ren_h;
    this->sdl2->SDL_GetWindowSize(this->win, &win_w, &win_h);
    if (this->sdl2->SDL_GetRendererOutputSize(this->ren, &ren_w, &ren_h) < 0) {
        _OMG_LOG_INFO(omg_base, "Failed to get renderer size (", this->sdl2->SDL_GetError(), ")");
        return;
    }
    base->size.w = (float)ren_w;
    base->size.h = (float)ren_h;
    base->scale.x = base->size.w / (float)win_w;
    base->scale.y = base->size.h / (float)win_h;
}

bool omg_renderer_sdl2_destroy(OMG_RendererSdl2* this) {
    if (base->inited) {
        this->sdl2->SDL_DestroyRenderer(this->ren);
        this->ren = NULL;
        base->inited = false;
    }
    return false;
}

int omg_renderer_sdl2_driver_from_name(OMG_RendererSdl2* this, const char* name) {
    if (OMG_ISNULL(name))
        return OMG_REN_DRIVER_UNKNOWN;
    if (!omg_base->std->strcmp(name, "direct3d"))
        return OMG_REN_DRIVER_D3D9;
    if (!omg_base->std->strcmp(name, "direct3d11"))
        return OMG_REN_DRIVER_D3D11;
    if (!omg_base->std->strcmp(name, "direct3d12"))
        return OMG_REN_DRIVER_D3D12;
    if (!omg_base->std->strcmp(name, "opengl"))
        return OMG_REN_DRIVER_OPENGL;
    if (!omg_base->std->strcmp(name, "opengles"))
        return OMG_REN_DRIVER_OPENGLES;
    if (!omg_base->std->strcmp(name, "opengles2"))
        return OMG_REN_DRIVER_OPENGLES;
    if (!omg_base->std->strcmp(name, "software"))
        return OMG_REN_DRIVER_SOFTWARE;
    return OMG_REN_DRIVER_UNKNOWN;
}

int omg_renderer_sdl2_get_renderer_id(OMG_RendererSdl2* this, int driver) {
    for (int i = 0; i < this->id_cache[9]; i++) {
        if (this->id_cache[i] == driver)
            return i;
    }
    return -1;
}

int omg_renderer_sdl2_get_supported_drivers(OMG_RendererSdl2* this) {
    if (this->id_cache[0] >= 0)
        return this->id_cache[8];
    int res = OMG_REN_DRIVER_NONE;
    int num_drivers = this->sdl2->SDL_GetNumRenderDrivers();
    if (num_drivers < 0) {
        _OMG_LOG_ERROR(omg_base, "Failed to get number of render drivers (", this->sdl2->SDL_GetError(), ")");
        return res;
    }
    if (num_drivers > 8)
        num_drivers = 8;
    this->id_cache[9] = num_drivers;
    SDL_RendererInfo info;
    for (int i = 0; i < num_drivers; i++) {
        if (this->sdl2->SDL_GetRenderDriverInfo(i, &info) < 0) {
            this->id_cache[i] = OMG_REN_DRIVER_NONE;
            continue;
        }
        this->id_cache[i] = omg_renderer_sdl2_driver_from_name(this, info.name);
        res |= this->id_cache[i];
    }
    this->id_cache[8] = res;
    return res;
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
    omg_renderer_sdl2_get_supported_drivers(this);
    base->_on_update_window_size = omg_renderer_sdl2_update_scale;
    base->destroy = omg_renderer_sdl2_destroy;
    base->clear = omg_renderer_sdl2_clear;
    base->flip = omg_renderer_sdl2_flip;
    OMG_END_POINTER_CAST();
    base->type = OMG_REN_TYPE_SDL2;
    int sdl2_driver;
    if (base->driver == OMG_REN_DRIVER_AUTO) {
        sdl2_driver = -1;
        // TODO: d3d12 since windows 10, d3d11 since vista or 7
        // TODO: Force OpenGLES on Android
        if (this->id_cache[8] & OMG_REN_DRIVER_D3D12)
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_D3D12);
        else if (this->id_cache[8] & OMG_REN_DRIVER_D3D11)
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_D3D11);
        else if (this->id_cache[8] & OMG_REN_DRIVER_D3D9)
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_D3D9);
        else if (!OMG_IS_ANDROID && (this->id_cache[8] & OMG_REN_DRIVER_OPENGL))
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_OPENGL);
        else if (this->id_cache[8] & OMG_REN_DRIVER_OPENGLES2)
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_OPENGLES2);
        else if (this->id_cache[8] & OMG_REN_DRIVER_OPENGLES)
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_OPENGLES);
        else if (this->id_cache[8] & OMG_REN_DRIVER_SOFTWARE)
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_SOFTWARE);
    }
    else
        sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, base->driver);
    this->ren = this->sdl2->SDL_CreateRenderer(
        this->win,
        sdl2_driver,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | (win_base->vsync ? SDL_RENDERER_PRESENTVSYNC : 0)
    );
    // Ugly Hack
    if (OMG_ISNULL(this->ren) && (sdl2_driver == omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_D3D12))) {
        base->driver = OMG_REN_DRIVER_D3D11;
        return omg_renderer_sdl2_init(this);
    }
    else if (OMG_ISNULL(this->ren) && (sdl2_driver == omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_D3D11))) {
        base->driver = OMG_REN_DRIVER_D3D9;
        return omg_renderer_sdl2_init(this);
    }
    if (OMG_ISNULL(this->ren)) {
        _OMG_LOG_ERROR(omg_base, "Failed to create SDL2 renderer (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    SDL_RendererInfo info;
    if (this->sdl2->SDL_GetRendererInfo(this->ren, &info) < 0)
        _OMG_LOG_WARN(omg_base, "Failed to get renderer info (", this->sdl2->SDL_GetError(), ")");
    else {
        base->driver = omg_renderer_sdl2_driver_from_name(this, info.name);
        _OMG_LOG_INFO(omg_base, "SDL2 renderer created successfuly with ", info.name, " driver");
    }
    omg_renderer_sdl2_update_scale(this);
    base->inited = true;
    return false;
}
#endif
