#pragma once
#include <omega/config.h>
#include <omega/ostd.h>

typedef struct {
    /* Allocated size (not length!) */
    size_t size;
    /* Length */
    size_t len;
    /* Chunk size (in elements) */
    int chunk_size;
    /* Data pointer */
    void* data;
} OMG_ArrayBase;

#define OMG_Array(array_type) struct { \
    /* Allocated size (not length!) */ \
    size_t size; \
    /* Length */ \
    size_t len; \
    /* Chunk size (in bytes) */ \
    int chunk_size; \
    /* Data pointer */ \
    array_type* data; \
}

#define OMG_ARRAY_INIT(this, initial_len, chunk_size) omg_array_init((OMG_ArrayBase*)(this), initial_len, (size_t)sizeof(*(this)->data), chunk_size)
#define OMG_ARRAY_DESTROY(this) omg_array_destroy((OMG_ArrayBase*)(this));

OMG_API bool omg_array_init(OMG_ArrayBase* this, size_t initial_len, size_t elem_size, int chunk_size);
OMG_API bool omg_array_destroy(OMG_ArrayBase* this);
