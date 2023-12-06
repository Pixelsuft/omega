#include <omega/config.h>
#include <omega/ostd.h>
#include <omega/memory.h>
#if OMG_SUPPORT_WIN
#include <omega/omega_win.h>
#endif
#if OMG_HAS_STD
#include <string.h>
#endif
#define mem ((OMG_Memory*)omg_def_std->memory_allocator)

// Some of the C std is taken from:
// https://github.com/libsdl-org/SDL/blob/main/src/stdlib/SDL_string.c

static const char ntoa_table[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
};
static OMG_Std* omg_def_std = NULL;

#if OMG_IMPL_BASIC_STD_FUNCS
void* omg_std_memset(void* dst, register int val, register size_t len) {
    register unsigned char *ptr = (unsigned char*)dst;
    while (len-- > 0)
        *ptr++ = val;
    return dst;
}

void* omg_std_memmove(void* dst, const void* src, size_t len) {
    char *srcp = (char *)src;
    char *dstp = (char *)dst;
    if (src < dst) {
        srcp += len - 1;
        dstp += len - 1;
        while (len--) {
            *dstp-- = *srcp--;
        }
    } else {
        while (len--) {
            *dstp++ = *srcp++;
        }
    }
    return dst;
}

void* omg_std_memcpy(void* dst, const void* src, size_t len) {
    char *d = dst;
    const char *s = src;
    while (len--)
        *d++ = *s++;
    return dst;
}

int omg_std_memcmp(const void* s1, const void* s2, size_t len) {
    char *s1p = (char *)s1;
    char *s2p = (char *)s2;
    while (len--) {
        if (*s1p != *s2p) {
            return *s1p - *s2p;
        }
        ++s1p;
        ++s2p;
    }
    return 0;
}

size_t omg_std_strlen(const char* src) {
    if (OMG_ISNULL(src))
        return 0;
    char* temp_counter = (char*)src;
    while (*temp_counter)
        temp_counter++;
    return (size_t)temp_counter - (size_t)src;
}

size_t omg_std_strnlen(const char* src, size_t max_len) {
    if (OMG_ISNULL(src))
        return 0;
    char* ptr_limit = (char*)((size_t)src + max_len);
    char* temp_counter = (char*)src;
    while (temp_counter < ptr_limit && *temp_counter)
        temp_counter++;
    return (size_t)temp_counter - (size_t)src;
}
#endif

size_t omg_std_static_strlen(const char* src) {
    if (OMG_ISNULL(src))
        return 0;
    if (OMG_ISNOTNULL(omg_def_std))
        return omg_def_std->strlen(src);
    char* temp_counter = (char*)src;
    while (*temp_counter)
        temp_counter++;
    return (size_t)temp_counter - (size_t)src;
}

size_t omg_std_wcslen(const wchar_t* src) {
    if (OMG_ISNULL(src))
        return 0;
    wchar_t* temp_counter = (wchar_t*)src;
    while (*temp_counter) {
        temp_counter++;
    }
    return ((size_t)temp_counter - (size_t)src) >> 1;
}

char* omg_std_strrev(char* str) {
    omg_std_str_reverse(str, omg_def_std->strlen(str));
    return str;
}

char* omg_std_itoa(int value, char* string, int radix) {
    return omg_def_std->ltoa((long)value, string, radix);
}

char* omg_std_uitoa(unsigned int value, char* string, int radix) {
    return omg_def_std->ultoa((unsigned long)value, string, radix);
}

char* omg_std_ltoa(long value, char* string, int radix) {
    char* bufp = string;
    if (value < 0) {
        *bufp++ = '-';
        omg_def_std->ultoa((unsigned long)(-value), bufp, radix);
    } else {
        omg_def_std->ultoa((unsigned long)value, bufp, radix);
    }
    return string;
}

char* omg_std_ultoa(unsigned long value, char* string, int radix) {
    char* bufp = string;
    if (value) {
        while (value > 0) {
            *bufp++ = ntoa_table[value % radix];
            value /= radix;
        }
    } else {
        *bufp++ = '0';
    }
    *bufp = '\0';
    // omg_std_str_reverse maybe?
    omg_def_std->strrev(string);
    return string;
}

char* omg_std_lltoa(int64_t value, char* string, int radix) {
    char* bufp = string;
    if (value < 0) {
        *bufp++ = '-';
        omg_def_std->ulltoa((uint64_t)(-value), bufp, radix);
    } else {
        omg_def_std->ulltoa((uint64_t)value, bufp, radix);
    }
    return string;
}

char* omg_std_ulltoa(uint64_t value, char* string, int radix) {
    char* bufp = string;
    if (value) {
        while (value > 0) {
            *bufp++ = ntoa_table[value % radix];
            value /= radix;
        }
    } else {
        *bufp++ = '0';
    }
    *bufp = '\0';
    omg_def_std->strrev(string);
    return string;
}

size_t omg_std_utf8strlen(const char* src) {
    size_t retval = 0;
    const char* p = src;
    unsigned char ch;
    while ((ch = *(p++)) != 0) {
        if ((ch & 0xc0) != 0x80) {
            retval++;
        }
    }
    return retval;
}

size_t omg_std_utf8strnlen(const char* src, size_t max_len) {
    size_t retval = 0;
    const char* p = src;
    unsigned char ch;
    while ((ch = *(p++)) != 0 && max_len-- > 0) {
        if ((ch & 0xc0) != 0x80) {
            retval++;
        }
    }
    return retval;
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

OMG_Std* omg_std_get_default_handle(void) {
    return omg_def_std;
}

void omg_std_set_default_handle(OMG_Std* this) {
    omg_def_std = this;
}

void* omg_std_lib_load(const OMG_String* fn) {
    OMG_UNUSED(fn);
    return NULL;
}

void* omg_std_lib_func(void* lib, const OMG_String* func_name) {
    OMG_UNUSED(lib, func_name);
    return NULL;
}

bool omg_std_lib_free(void* lib) {
    OMG_UNUSED(lib);
    return false;
}

void omg_std_fill_defaults(OMG_Std* this) {
    this->lib_load = omg_std_lib_load;
    this->lib_func = omg_std_lib_func;
    this->lib_free = omg_std_lib_free;
#if OMG_IMPL_BASIC_STD_FUNCS
    this->memset = omg_std_memset;
    this->memcpy = omg_std_memcpy;
    this->memmove = omg_std_memmove;
    this->memcmp = omg_std_memcmp;
    this->strlen = omg_std_strlen;
    this->strnlen = omg_std_strnlen;
#else
    this->memset = memset;
    this->memcpy = memcpy;
    this->memmove = memmove;
    this->memcmp = memcmp;
    this->strlen = strlen;
    this->strnlen = strnlen;
#endif
    // These funcs are not that portable, so let's keep them here
    this->wcslen = omg_std_wcslen;
    this->strrev = omg_std_strrev;
    this->itoa = omg_std_itoa;
    this->uitoa = omg_std_uitoa;
    this->ltoa = omg_std_ltoa;
    this->ultoa = omg_std_ultoa;
    this->lltoa = omg_std_lltoa;
    this->ulltoa = omg_std_ulltoa;
    this->utf8strlen = omg_std_utf8strlen;
    this->utf8strnlen = omg_std_utf8strnlen;
}

bool omg_string_add_double(OMG_String* this, const double double_to_add) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (omg_string_ensure_free_len(this, 21))
        return true;
    omg_def_std->itoa((int)double_to_add, this->ptr + this->len, 10);
    this->len += omg_def_std->strnlen(this->ptr + this->len, 20);
    // TODO: round
    int double_prec = (int)((double_to_add - (double)((int)double_to_add)) * 1000000.0);
    if (double_prec <= 0)
        return false;
    while (!(double_prec % 10))
        double_prec /= 10;
    this->ptr[this->len] = '.';
    omg_def_std->itoa(double_prec, this->ptr + this->len + 1, 10);
    this->len += omg_def_std->strnlen(this->ptr + this->len + 1, 20) + 1;
    return false;
}

bool omg_string_add_float(OMG_String* this, const float float_to_add) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (omg_string_ensure_free_len(this, 21))
        return true;
    omg_def_std->itoa((int)float_to_add, this->ptr + this->len, 10);
    this->len += omg_def_std->strnlen(this->ptr + this->len, 20);
    // TODO: round
    int float_prec = (int)((float_to_add - (float)((int)float_to_add)) * 10000.0f);
    if (float_prec <= 0)
        return false;
    while (!(float_prec % 10))
        float_prec /= 10;
    this->ptr[this->len] = '.';
    omg_def_std->itoa(float_prec, this->ptr + this->len + 1, 10);
    this->len += omg_def_std->strnlen(this->ptr + this->len + 1, 20) + 1;
    return false;
}

bool omg_string_add_pointer(OMG_String* this, const void* pointer_to_add) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (omg_string_ensure_free_len(this, 41))
        return true;
    omg_def_std->ulltoa((uint64_t)((size_t)pointer_to_add), this->ptr + this->len, 16);
    this->len += omg_def_std->strnlen(this->ptr + this->len, 40);
    return false;
}

bool omg_string_add_ulonglong(OMG_String* this, const uint64_t ulonglong_to_add) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (omg_string_ensure_free_len(this, 41))
        return true;
    omg_def_std->ulltoa(ulonglong_to_add, this->ptr + this->len, 10);
    this->len += omg_def_std->strnlen(this->ptr + this->len, 40);
    return false;
}

bool omg_string_add_longlong(OMG_String* this, const int64_t longlong_to_add) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (omg_string_ensure_free_len(this, 41))
        return true;
    omg_def_std->lltoa(longlong_to_add, this->ptr + this->len, 10);
    this->len += omg_def_std->strnlen(this->ptr + this->len, 40);
    return false;
}

bool omg_string_add_ulong(OMG_String* this, const unsigned long ulong_to_add) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (omg_string_ensure_free_len(this, 21))
        return true;
    omg_def_std->ultoa(ulong_to_add, this->ptr + this->len, 10);
    this->len += omg_def_std->strnlen(this->ptr + this->len, 40);
    return false;
}

bool omg_string_add_long(OMG_String* this, const long long_to_add) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (omg_string_ensure_free_len(this, 21))
        return true;
    omg_def_std->ltoa(long_to_add, this->ptr + this->len, 10);
    this->len += omg_def_std->strnlen(this->ptr + this->len, 20);
    return false;
}

bool omg_string_add_uint(OMG_String* this, const unsigned int uint_to_add) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (omg_string_ensure_free_len(this, 11))
        return true;
    omg_def_std->uitoa(uint_to_add, this->ptr + this->len, 10);
    this->len += omg_def_std->strnlen(this->ptr + this->len, 20);
    return false;
}

bool omg_string_add_int(OMG_String* this, const int int_to_add) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (omg_string_ensure_free_len(this, 11))
        return true;
    omg_def_std->itoa(int_to_add, this->ptr + this->len, 10);
    this->len += omg_def_std->strnlen(this->ptr + this->len, 20);
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

bool omg_string_add_wchar_p(OMG_String* this, const wchar_t* wstr_to_add) {
#if OMG_SUPPORT_WIN
    if (this->type < OMG_STRING_BUFFER || OMG_ISNULL(wstr_to_add))
        return true;
    OMG_OmegaWin* omg = (OMG_OmegaWin*)omg_get_default_omega();
    if (OMG_ISNULL(omg) || (((OMG_Omega*)omg)->type != OMG_OMEGA_TYPE_WIN))
        return true;
    size_t need_len = omg_def_std->wcslen(wstr_to_add);
    size_t need_count;
    _OMG_WIN_GET_DECODE_SIZE(need_count, wstr_to_add, omg->k32, need_len);
    if ((need_count == 0) || omg_string_ensure_free_len(this, need_count))
        return true;
    int res = omg->k32->WideCharToMultiByte(WIN_CP_UTF8, 0, wstr_to_add, need_len, this->ptr + this->len, need_count, NULL, NULL);
    if (res > 0) {
        this->len += (size_t)res;
        return false;
    }
    return true;
#else
    // TODO: non-weendoes way
    UNUSED(this, wstr_to_add);
    return true;
#endif
}

bool omg_string_init_dynamic(OMG_String* this, const OMG_String* base) {
    if (OMG_ISNULL(omg_def_std))
        return true;
    if (OMG_ISNULL(base)) {
        this->len = 0;
        this->size = OMG_STRING_CHUNK_SIZE;
        this->ptr = _OMG_INTERNAL_MALLOC(mem, OMG_STRING_CHUNK_SIZE);
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
    this->ptr = _OMG_INTERNAL_MALLOC(mem, this->size);
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
