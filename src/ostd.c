#include <omega/ostd.h>
#include <omega/memory.h>
#define mem ((OMG_Memory*)omg_def_std->memory_allocator)

static OMG_Std* omg_def_std = NULL;

// TODO: Should we redirect to omg_def_std if not the same?
void* omg_memset(void* dest, register int val, register size_t len) {
    register unsigned char *ptr = (unsigned char*)dest;
    while (len-- > 0)
        *ptr++ = val;
    return dest;
}

void* omg_memcpy(void* dest, const void* src, size_t len) {
    char *d = dest;
    const char *s = src;
    while (len--)
        *d++ = *s++;
    return dest;
}

size_t omg_strlen(const char* src) {
    if (OMG_ISNULL(src))
        return 0;
    char* temp_counter = (char*)src;
    while (*temp_counter)
        temp_counter++;
    return (size_t)temp_counter - (size_t)src;
}

void omg_std_set_default_handle(OMG_Std* this) {
    omg_def_std = this;
}

void omg_std_fill_defaults(OMG_Std* this) {
    this->memset = omg_memset;
    this->memcpy = omg_memcpy;
    this->strlen = omg_strlen;
}

bool omg_string_init_dynamic(OMG_String* this, const OMG_String* base) {
    if (OMG_ISNULL(base)) {
        this->len = 0;
        this->size = OMG_STRING_CHUNK_SIZE;
        this->ptr = OMG_MALLOC(mem, OMG_STRING_CHUNK_SIZE);
        if (OMG_ISNULL(this->ptr)) {
            this->size = 0;
            this->type = OMG_STRING_NONE;
            return true;
        }
        this->type = OMG_STRING_DYNAMIC;
        return false;
    }
    this->len = base->len;
    this->size = (this->len % OMG_STRING_CHUNK_SIZE) ? ((this->len / OMG_STRING_CHUNK_SIZE + 1) * OMG_STRING_CHUNK_SIZE) : this->len;
    if (this->size < OMG_STRING_CHUNK_SIZE)
        this->size = OMG_STRING_CHUNK_SIZE;
    this->ptr = OMG_MALLOC(mem, this->size);
    if (OMG_ISNULL(this->ptr)) {
        this->size = 0;
        this->type = OMG_STRING_NONE;
        return true;
    }
    omg_def_std->memcpy(this->ptr, base->ptr, this->len);
    return false;
}

bool omg_string_destroy(OMG_String* this) {
    bool res = false;
    if (this->type == OMG_STRING_DYNAMIC && OMG_ISNOTNULL(this->ptr)) {
        res = OMG_FREE(mem, this->ptr);
        this->ptr = NULL;
    }
    this->type = OMG_STRING_NONE;
    return res;
}
