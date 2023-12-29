#pragma once
#include <omega/memory.h>

#if OMG_SUPPORT_SDL2
#include <omega/api_sdl2.h>

typedef struct {
    OMG_Memory parent;
    OMG_Sdl2* sdl2;
    void* omg;
} OMG_MemorySdl2;

OMG_API OMG_MemorySdl2* omg_memory_sdl2_create(void* omg, OMG_Sdl2* sdl2);
#if OMG_EXPORT_SHIT
OMG_API size_t omg_memory_sdl2_get_alloc_count(OMG_MemorySdl2* this);
OMG_API bool omg_memory_sdl2_destroy(OMG_MemorySdl2* this);
OMG_API void* omg_memory_sdl2_alloc(OMG_MemorySdl2* this, OMG_MemoryExtra extra);
OMG_API void* omg_memory_sdl2_realloc(OMG_MemorySdl2* this, void* ptr, size_t size);
OMG_API bool omg_memory_sdl2_free(OMG_MemorySdl2* this, void* ptr);
#endif
#endif
