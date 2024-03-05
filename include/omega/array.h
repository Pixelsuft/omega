#pragma once
#include <omega/config.h>
#include <omega/ostd.h>

typedef struct {
    /* Data pointer */
    void* data;
    /* Allocated size (not length!) */
    size_t size;
    /* Length */
    size_t len;
    /* Chunk size (in elements) */
    int chunk_size;
} OMG_ArrayBase;

#define OMG_Array(array_type) struct { \
    /* Data pointer */ \
    array_type* data; \
    /* Allocated size (not length!) */ \
    size_t size; \
    /* Length */ \
    size_t len; \
    /* Chunk size (in bytes) */ \
    int chunk_size; \
}

#define OMG_ARRAY_INIT(this, initial_len, chunk_size) omg_array_init((OMG_ArrayBase*)(this), initial_len, (size_t)sizeof(*(this)->data), chunk_size)
#define OMG_ARRAY_DESTROY(this) omg_array_destroy((OMG_ArrayBase*)(this))
#define OMG_ARRAY_ADD_CHUNK(this) omg_array_add_chunk((OMG_ArrayBase*)(this))
#define OMG_ARRAY_SET_LEN(this, need_len, trim) omg_array_set_len((OMG_ArrayBase*)(this), need_len, (size_t)sizeof(*(this)->data), trim)
#define OMG_ARRAY_PUSH(this, new_elem) omg_array_push((OMG_ArrayBase*)(this), (size_t)sizeof(*(this)->data), &new_elem)
#define OMG_ARRAY_REMOVE(this, elem_id, trim) omg_array_remove((OMG_ArrayBase*)(this), elem_id, (size_t)sizeof(*(this)->data), trim)
#define OMG_ARRAY_CLEAN(this) omg_array_remove((OMG_ArrayBase*)(this), (size_t)sizeof(*(this)->data))

OMG_API bool omg_array_init(OMG_ArrayBase* this, size_t initial_len, size_t elem_size, int chunk_size);
OMG_API bool omg_array_set_len(OMG_ArrayBase* this, size_t need_len, size_t elem_size, bool trim);
OMG_API bool omg_array_push(OMG_ArrayBase* this, size_t elem_size, const void* need_elem);
OMG_API bool omg_array_add_chunk(OMG_ArrayBase* this);
OMG_API bool omg_array_remove(OMG_ArrayBase* this, size_t elem_id, size_t elem_size, bool trim);
OMG_API bool omg_array_clean(OMG_ArrayBase* this, size_t elem_size);
OMG_API bool omg_array_destroy(OMG_ArrayBase* this);
