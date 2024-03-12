#include <omega/array.h>
#include <omega/memory.h>
#include <omega/omega.h>

static OMG_Memory* omg_mem = NULL;
static OMG_Std* omg_std = NULL;

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

bool omg_array_add_chunk(OMG_ArrayBase* this) {
    void* new_ptr = OMG_REALLOC(omg_mem, this->data, this->size + (size_t)this->chunk_size);
    if (OMG_ISNULL(new_ptr))
        return true;
    this->size += (size_t)this->chunk_size;
    this->data = new_ptr;
    return false;
}

bool omg_array_set_len(OMG_ArrayBase* this, size_t need_len, size_t elem_size, bool trim) {
    size_t need_size = need_len * elem_size;
    if (need_size % (size_t)this->chunk_size || 1) {
        need_size /= (size_t)this->chunk_size;
        need_size++;
        need_size *= (size_t)this->chunk_size;
    }
    if ((need_size < this->size) && !trim) {
        this->len = need_len;
        return false;
    }
    void* new_ptr = OMG_REALLOC(omg_mem, this->data, need_size);
    /* if (new_ptr != this->data) {
        printf("NEW POINTER GUYS: %p %p\n", new_ptr, this->data);
    } */
    if (OMG_ISNULL(new_ptr)) {
        return true;
    }
    this->size = need_size;
    this->data = new_ptr;
    this->len = need_len;
    return false;
}

bool omg_array_push(OMG_ArrayBase* this, size_t elem_size, const void* need_elem) {
    if (((this->len + 1) * elem_size) > this->size) {
        if (omg_array_add_chunk(this))
            return true;
    }
    omg_std->memcpy((void*)((size_t)(this->data) + (this->len * elem_size)), need_elem, elem_size);
    this->len++;
    return false;
}

bool omg_array_push_dummy(OMG_ArrayBase* this, size_t elem_size) {
    if (((this->len + 1) * elem_size) > this->size) {
        if (omg_array_add_chunk(this))
            return true;
    }
    this->len++;
    return false;
}

bool omg_array_remove(OMG_ArrayBase* this, size_t elem_id, size_t elem_size, bool trim) {
    if ((elem_id + 1) >= this->len) {
        this->len--;
        if (trim)
            return omg_array_clean(this, elem_size);
        return false;
    }
    omg_std->memmove(
        (void*)((size_t)(this->data) + (elem_id * elem_size)),
        (void*)((size_t)(this->data) + ((elem_id + 1) * elem_size)),
        (this->len - elem_id - 1) * elem_size
    );
    this->len--;
    if (trim)
        return omg_array_clean(this, elem_size);
    return false;
}

bool omg_array_reserve_len(OMG_ArrayBase* this, size_t need_len, size_t elem_size) {
    size_t need_size = need_len * elem_size;
    if (need_size % (size_t)this->chunk_size || 1) {
        need_size /= (size_t)this->chunk_size;
        need_size++;
        need_size *= (size_t)this->chunk_size;
    }
    if (this->size >= need_size)
        return false;
    void* new_ptr = OMG_REALLOC(omg_mem, this->data, need_size);
    if (OMG_ISNULL(new_ptr))
        return true;
    this->data = new_ptr;
    this->size = need_size;
    return false;
}

bool omg_array_clean(OMG_ArrayBase* this, size_t elem_size) {
    size_t need_size = this->len * elem_size;
    if (need_size % (size_t)this->chunk_size || 1) {
        need_size /= (size_t)this->chunk_size;
        need_size++;
        need_size *= (size_t)this->chunk_size;
    }
    if (this->size >= need_size) {
        void* new_ptr = OMG_REALLOC(omg_mem, this->data, need_size);
        if (OMG_ISNULL(new_ptr))
            return true;
        this->data = new_ptr;
        this->size = need_size;
    }
    return false;
}

bool omg_array_init(OMG_ArrayBase* this, size_t initial_len, size_t elem_size, int chunk_size) {
    if (chunk_size <= 0)
        chunk_size = (int)elem_size;
    this->chunk_size = chunk_size;
    this->len = initial_len;
    this->size = initial_len * elem_size;
    if ((this->size % (size_t)chunk_size) > 0) {
        this->size /= (size_t)chunk_size;
        this->size++;
        this->size *= (size_t)chunk_size;
    }
    omg_mem = omg_get_default_omega()->mem;
    omg_std = omg_get_default_omega()->std;
    this->data = OMG_MALLOC(omg_mem, this->size);
    if (OMG_ISNULL(this->data)) {
        this->chunk_size = 0;
        this->size = 0;
        this->len = 0;
        return true;
    }
    return false;
}
