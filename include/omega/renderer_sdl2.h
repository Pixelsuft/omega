#pragma once
#include <omega/api_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/renderer.h>
#define _OMG_SDL2_DRAW_COLOR_WARN() _OMG_LOG_WARN(omg_base, "Failed to set draw color (", this->sdl2->SDL_GetError(), ")")
#define _OMG_SDL2_DRAW_BLEND_WARN() _OMG_LOG_WARN(omg_base, "Failed to set draw blend mode (", this->sdl2->SDL_GetError(), ")")
#define _OMG_SDL2_SCALE_WARN() _OMG_LOG_WARN(omg_base, "Failed to set render scale (", this->sdl2->SDL_GetError(), ")")
#define _OMG_SDL2_COPY_WARN() _OMG_LOG_WARN(omg_base, "Failed to render copy (", this->sdl2->SDL_GetError(), ")")

typedef struct {
    OMG_Renderer parent;
    OMG_Sdl2* sdl2;
    SDL_Window* win;
    SDL_Renderer* ren;
    int id_cache[10];
    int blend_cache;
    int win_build_num;
} OMG_RendererSdl2;

OMG_API bool omg_renderer_sdl2_init(OMG_RendererSdl2* this);
OMG_API bool omg_renderer_sdl2_destroy(OMG_RendererSdl2* this);
OMG_API int omg_renderer_sdl2_get_supported_drivers(OMG_RendererSdl2* this);
#if OMG_EXPORT_SHIT
#include <omega/font.h>
#include <omega/texture_sdl2.h>

OMG_TextureSdl2* omg_renderer_sdl2_font_render(OMG_RendererSdl2* this, OMG_TextureSdl2* tex, OMG_Font* font, const OMG_String* text, const OMG_Color* bg, const OMG_Color* fg, OMG_FRect* rect);
OMG_API int omg_renderer_sdl2_get_renderer_id(OMG_RendererSdl2* this, int driver);
OMG_API int omg_renderer_sdl2_driver_from_name(OMG_RendererSdl2* this, const char* name);
OMG_API void omg_renderer_sdl2_update_scale(OMG_RendererSdl2* this);
OMG_API bool omg_renderer_sdl2_clear(OMG_RendererSdl2* this, const OMG_Color* col);
OMG_API bool omg_renderer_sdl2_flip(OMG_RendererSdl2* this);
OMG_API bool omg_renderer_sdl2_set_scale(OMG_RendererSdl2* this, const OMG_FPoint* offset, const OMG_FPoint* scale);
OMG_API bool omg_renderer_sdl2_set_target(OMG_RendererSdl2* this, OMG_TextureSdl2* tex);
OMG_API bool omg_renderer_sdl2_draw_point(OMG_RendererSdl2* this, const OMG_FPoint* pos, const OMG_Color* col);
OMG_API bool omg_renderer_sdl2_draw_line(OMG_RendererSdl2* this, const OMG_FRect* start_end, const OMG_Color* col);
OMG_API bool omg_renderer_sdl2_draw_rect(OMG_RendererSdl2* this, const OMG_FRect* rect, const OMG_Color* col);
OMG_API bool omg_renderer_sdl2_fill_rect(OMG_RendererSdl2* this, const OMG_FRect* rect, const OMG_Color* col);
OMG_API bool omg_renderer_sdl2_draw_ellipse(OMG_RendererSdl2* this, const OMG_FPoint* pos, float rx, float ry, const OMG_Color* col);
OMG_API bool omg_renderer_sdl2_fill_ellipse(OMG_RendererSdl2* this, const OMG_FPoint* pos, float rx, float ry, const OMG_Color* col);
OMG_API OMG_TextureSdl2* omg_renderer_sdl2_tex_from_surf(OMG_RendererSdl2* this, OMG_TextureSdl2* tex, OMG_Surface* surf, bool destroy_surf);
OMG_API OMG_TextureSdl2* omg_renderer_sdl2_tex_create(OMG_RendererSdl2* this, OMG_TextureSdl2* tex, const OMG_FPoint* size, int access, bool has_alpha);
OMG_API bool omg_renderer_sdl2_tex_destroy(OMG_RendererSdl2* this, OMG_TextureSdl2* tex);
OMG_API bool omg_renderer_sdl2_copy(OMG_RendererSdl2* this, OMG_TextureSdl2* tex, const OMG_FPoint* pos);
OMG_API bool omg_renderer_sdl2_set_vsync(OMG_RendererSdl2* this, bool enabled);
OMG_API bool omg_renderer_sdl2_copy_ex(OMG_RendererSdl2* this, OMG_TextureSdl2* tex, const OMG_FRect* src, const OMG_FRect* dst, const OMG_FPoint* origin, const double rot);
OMG_API bool omg_renderer_sdl2_tex_set_scale_mode(OMG_RendererSdl2* this, OMG_TextureSdl2* tex, int scale_mode);
OMG_API bool omg_renderer_sdl2_tex_set_color_mod(OMG_RendererSdl2* this, OMG_TextureSdl2* tex, const OMG_Color* col);
OMG_API bool omg_renderer_sdl2_tex_set_blend_mode(OMG_RendererSdl2* this, OMG_TextureSdl2* tex, int blend_mode);
OMG_API bool omg_renderer_sdl2_draw_line_ex(OMG_RendererSdl2* this, const OMG_FRect* start_end, float thick, const OMG_Color* col);
OMG_API bool omg_renderer_sdl2_set_clip_rect(OMG_RendererSdl2* this, const OMG_FRect* clip_rect);
OMG_API bool omg_renderer_sdl2_fill_rect_ex(OMG_RendererSdl2* this, const OMG_FRect* rect, float roundness, const OMG_Color* col);
OMG_API bool omg_renderer_sdl2_draw_rect_ex(OMG_RendererSdl2* this, const OMG_FRect* rect, float roundness, float thick, const OMG_Color* col);
OMG_API bool omg_renderer_sdl2_font_render_to(OMG_RendererSdl2* this, const OMG_FPoint* pos, OMG_Font* font, const OMG_String* text, const OMG_Color* bg, const OMG_Color* fg, OMG_FRect* rect);
#endif
#endif
