#pragma once
#include <omega/omega.h>
#include <omega/scene.h>
#include <omega/scene_objects.h>

typedef struct {
    OMG_Object parent;
    OMG_ObjectAnimSprite a;
    OMG_AnimSpriteData d;
} Player;
