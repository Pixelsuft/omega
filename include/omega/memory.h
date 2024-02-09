#pragma once
#include <omega/config.h>
#include <omega/ostd.h>

#define _OMG_MEMORY_ALLOC_INFO(extra) "Allocation info: at file ", (extra)->filename, ", in line ", (uint32_t)((extra)->line), ", at function ", (extra)->func
#define _OMG_MEMORY_NULL_FREE_WARN "Attempted to free null ptr"

#if OMG_DEBUG_MEM
typedef struct {
    size_t size;
    const char* filename;
    const char* func;
    size_t line;
    bool is_allocated;
} OMG_MemoryExtra;
// TODO: check if this works in c++
#define OMG_MALLOC(mem, size) (mem)->alloc(mem, (OMG_MemoryExtra){ (size_t)(size), __FILE__, __func__, __LINE__, false })
#define _OMG_INTERNAL_MALLOC(mem, size) (mem)->alloc(mem, (OMG_MemoryExtra){ (size_t)(size), NULL, NULL, 0, false })
#else
typedef size_t OMG_MemoryExtra;
#define OMG_MALLOC(mem, size) (mem)->alloc(mem, (OMG_MemoryExtra)(size))
#define _OMG_INTERNAL_MALLOC(mem, size) (mem)->alloc(mem, (OMG_MemoryExtra)(size))
#endif
#define OMG_REALLOC(mem, ptr, size) (mem)->realloc(mem, ptr, (size_t)(size))
#define OMG_FREE(mem, ptr) (mem)->free(mem, ptr)

typedef struct OMG_Memory {
    size_t alloc_count;
    size_t alloc_size;
    bool (*destroy)(struct OMG_Memory* this);
    void* (*alloc)(struct OMG_Memory* this, OMG_MemoryExtra extra);
    void* (*realloc)(struct OMG_Memory* this, void* ptr, size_t size);
    bool (*free)(struct OMG_Memory* this, void* ptr);
    size_t (*get_alloc_count)(struct OMG_Memory* this);
    bool is_allocated;
} OMG_Memory;

OMG_API bool omg_memory_init(OMG_Memory* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_memory_destroy(OMG_Memory* this);
OMG_API void* omg_memory_alloc(OMG_Memory* this, OMG_MemoryExtra extra);
OMG_API void* omg_memory_realloc(OMG_Memory* this, void* ptr, size_t size);
OMG_API bool omg_memory_free(OMG_Memory* this, void* ptr);
OMG_API size_t omg_memory_get_alloc_count(OMG_Memory* this);
#endif
