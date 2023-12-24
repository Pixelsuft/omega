#include <omega/renderer_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/window.h>
#include <omega/omega.h>
#define base ((OMG_Renderer*)this)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_renderer_sdl2_destroy(OMG_RendererSdl2* this) {
    if (base->inited) {
        base->inited = false;
    }
    return false;
}

bool omg_renderer_sdl2_init(OMG_RendererSdl2* this) {
    OMG_BEGIN_POINTER_CAST();
    omg_renderer_init(this);
    base->destroy = omg_renderer_sdl2_destroy;
    OMG_END_POINTER_CAST();
    base->type = OMG_REN_TYPE_SDL2;
    base->inited = true;
    return false;
}
#endif
