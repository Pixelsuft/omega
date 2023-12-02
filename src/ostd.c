#include <omega/ostd.h>
#include <omega/memory.h>
#define mem ((OMG_Memory*)omg_def_std->memory_allocator)

static OMG_Std* omg_def_std = NULL;

// TODO: Should we redirect to omg_def_std if not the same?
void* omg_std_memset(void* dest, register int val, register size_t len) {
    register unsigned char *ptr = (unsigned char*)dest;
    while (len-- > 0)
        *ptr++ = val;
    return dest;
}

void* omg_std_memcpy(void* dest, const void* src, size_t len) {
    char *d = dest;
    const char *s = src;
    while (len--)
        *d++ = *s++;
    return dest;
}

size_t omg_std_strlen(const char* src) {
    if (OMG_ISNULL(src))
        return 0;
    char* temp_counter = (char*)src;
    while (*temp_counter)
        temp_counter++;
    return (size_t)temp_counter - (size_t)src;
}

void omg_std_str_reverse(char* str, size_t length) {
    size_t start = 0;
    size_t end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char* omg_std_itoa(int value, char* buffer, int radix) {
    size_t i = 0;
    bool isNegative = false;
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return buffer;
    }
    if (value < 0 && radix == 10) {
        isNegative = true;
        value = -value;
    }
    while (value != 0) {
        int rem = value % radix;
        buffer[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value = value / radix;
    }
    if (isNegative)
        buffer[i++] = '-';
    buffer[i] = '\0';
    omg_std_str_reverse(buffer, i);
    return buffer;
}

OMG_Std* omg_std_get_default_handle(void) {
    return omg_def_std;
}

void omg_std_set_default_handle(OMG_Std* this) {
    omg_def_std = this;
}

void omg_std_fill_defaults(OMG_Std* this) {
    this->memset = omg_std_memset;
    this->memcpy = omg_std_memcpy;
    this->strlen = omg_std_strlen;
    this->itoa = omg_std_itoa;
}

bool omg_string_add_int(OMG_String* this, const int int_to_add) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (omg_string_ensure_free_len(this, 11))
        return true;
    omg_def_std->itoa(int_to_add, this->ptr + this->len, 10);
    this->len += omg_def_std->strlen(this->ptr + this->len);
    return false;
}

bool omg_string_add_char(OMG_String* this, const char char_to_add) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (this->size > this->len) {
        this->ptr[this->len] = char_to_add;
        this->len++;
        return false;
    }
    if (this->type < OMG_STRING_DYNAMIC)
        return true;
    if (omg_string_buffer_add_chunk(this))
        return true;
    this->ptr[this->len] = char_to_add;
    this->len++;
    return false;
}

bool omg_string_add_char_p(OMG_String* this, const char* str_to_add) {
    if (this->type < OMG_STRING_BUFFER || OMG_ISNULL(str_to_add))
        return true;
    size_t str_len = omg_def_std->strlen(str_to_add);
    if (this->size > (this->len + str_len)) {
        omg_def_std->memcpy(this->ptr + this->len, str_to_add, str_len);
        this->len += str_len;
        return false;
    }
    if (this->type < OMG_STRING_DYNAMIC)
        return true;
    if (omg_string_buffer_set_size(this, this->size + OMG_STRING_CALC_SIZE_BY_LENGTH(str_len)))
        return true;
    omg_def_std->memcpy(this->ptr + this->len, str_to_add, str_len);
    this->len += str_len;
    return false;
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
    this->size = OMG_STRING_CALC_SIZE_BY_LENGTH(this->len);
    if (this->size < OMG_STRING_CHUNK_SIZE)
        this->size = OMG_STRING_CHUNK_SIZE;
    this->ptr = OMG_MALLOC(mem, this->size);
    if (OMG_ISNULL(this->ptr)) {
        this->size = 0;
        this->type = OMG_STRING_NONE;
        return true;
    }
    this->type = OMG_STRING_DYNAMIC;
    omg_def_std->memcpy(this->ptr, base->ptr, this->len);
    return false;
}

bool omg_string_buffer_set_size(OMG_String* this, size_t new_size) {
    if (this->type < OMG_STRING_DYNAMIC)
        return true;
    char* res = OMG_REALLOC(mem, this->ptr, new_size);
    if (OMG_ISNULL(res))
        return true;
    this->size = new_size;
    this->ptr = res;
    return false;
}

bool omg_string_buffer_add_chunk(OMG_String* this) {
    return omg_string_buffer_set_size(this, this->size + OMG_STRING_CHUNK_SIZE);
}

bool omg_string_resize(OMG_String* this, size_t new_len) {
    if (this->type < OMG_STRING_DYNAMIC)
        return true;
    if (this->len == new_len)
        return false;
    size_t new_size = OMG_STRING_CALC_SIZE_BY_LENGTH(new_len);
    if (new_size != this->size) {
        char* res = OMG_REALLOC(mem, this->ptr, new_size);
        if (OMG_ISNULL(res))
            return true;
        this->size = new_size;
        this->ptr = res;
    }
    if (new_len > this->len) {
        omg_def_std->memset(this->ptr + this->len, 0, new_len - this->len);
    }
    this->len = new_len;
    return false;
}

bool omg_string_add(OMG_String* this, const OMG_String* new_str) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (this->size >= (this->len + new_str->len)) {
        omg_def_std->memcpy(this->ptr + this->len, new_str->ptr, new_str->len);
        this->len += new_str->len;
        return false;
    }
    if (this->type < OMG_STRING_DYNAMIC)
        return true;
    if (omg_string_buffer_set_size(this, this->size + new_str->size))
        return true;
    omg_def_std->memcpy(this->ptr + this->len, new_str->ptr, new_str->len);
    this->len += new_str->len;
    return false;
}

bool omg_string_ensure_null(OMG_String* this) {
    if (this->type < OMG_STRING_STATIC)
        return true;
    if (this->size > this->len && this->ptr[this->len] == '\0')
        return false;
    if (this->type == OMG_STRING_STATIC)
        return true;
    if (this->size > this->len) {
        this->ptr[this->len] = '\0';
        return false;
    }
    if (this->type < OMG_STRING_DYNAMIC)
        return true;
    if (omg_string_buffer_add_chunk(this))
        return true;
    this->ptr[this->len] = '\0';
    return false;
}

bool omg_string_ensure_free_len(OMG_String* this, size_t need_len) {
    if (this->size >= this->len + need_len)
        return false;
    if (this->type < OMG_STRING_DYNAMIC)
        return true;
    size_t need_size = OMG_STRING_CALC_SIZE_BY_LENGTH(need_len + this->len);
    if (omg_string_buffer_set_size(this, need_size))
        return true;
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
