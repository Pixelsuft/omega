#include <omega/array.h>
#include <omega/memory.h>
#include <omega/omega.h>

static OMG_Memory* omg_mem;

bool omg_array_destroy(OMG_ArrayBase* this) {
    if (OMG_ISNULL(this->data))
        return true;
    OMG_FREE(omg_mem, this->data);
    this->data = NULL;
    this->chunk_size = 0;
    this->size = 0;
    this->len = 0;
    return false;
}

bool omg_array_init(OMG_ArrayBase* this, size_t initial_len, size_t elem_size, int chunk_size) {
    if (chunk_size <= 0)
        chunk_size = 1;
    this->chunk_size = chunk_size;
    this->len = initial_len;
    this->size = initial_len;
    if (this->size % (size_t)chunk_size) {
        this->size /= (size_t)chunk_size;
        this->size *= (size_t)chunk_size;
        this->size++;
    }
    omg_mem = omg_get_default_omega()->mem;
    this->data = OMG_MALLOC(omg_mem, elem_size * this->size);
    if (OMG_ISNULL(this->data)) {
        this->chunk_size = 0;
        this->size = 0;
        this->len = 0;
        return true;
    }
    return false;
}
