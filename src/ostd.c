#include <omega/ostd.h>

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

OMG_API void omg_std_fill_defaults(OMG_Std* this) {
    this->memset = omg_memset;
    this->memcpy = omg_memcpy;
    this->strlen = omg_strlen;
}
