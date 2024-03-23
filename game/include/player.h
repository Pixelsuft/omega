#pragma once
#include <omega/omega.h>
#include <omega/scene.h>
#include <omega/scene_objects.h>

/*
Sprite anim info:
0) idle
1) fall
2) cassical run
3) jump
4) run
*/

typedef struct {
    OMG_Object parent;
    OMG_ObjectAnimSprite a;
    OMG_AnimSpriteData d;
} Player;
