#pragma once
#include <omega/memory.h>

#if OMG_SUPPORT_SDL2
#include <omega/sdl2.h>

typedef struct {
    OMG_Memory parent;
    OMG_Sdl2* k32;
    void* omg;
} OMG_MemoryWin;
#endif
