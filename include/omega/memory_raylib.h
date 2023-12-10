#pragma once
#include <omega/memory.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/api_raylib.h>

typedef struct {
    OMG_Memory parent;
    OMG_Raylib* raylib;
    void* omg;
} OMG_MemoryRaylib;

OMG_API OMG_MemoryRaylib* omg_memory_raylib_create(void* omg, OMG_Raylib* raylib);
#if OMG_EXPORT_SHIT
OMG_API bool omg_memory_raylib_destroy(OMG_MemoryRaylib* this);
OMG_API void* omg_memory_raylib_alloc(OMG_MemoryRaylib* this, OMG_MemoryExtra extra);
OMG_API void* omg_memory_raylib_realloc(OMG_MemoryRaylib* this, void* ptr, size_t size);
OMG_API bool omg_memory_raylib_free(OMG_MemoryRaylib* this, void* ptr);
#endif
#endif
