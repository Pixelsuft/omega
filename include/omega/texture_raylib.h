#pragma once
#include <omega/api_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/texture.h>

typedef struct {
    OMG_Texture parent;
    Texture2D* tex;
    RenderTexture2D target;
    Color tint;
    bool is_target;
} OMG_TextureRaylib;
#endif
