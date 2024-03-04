#include <omega/config.h>
#include <omega/ostd.h>
#include <omega/memory.h>
#include <omega/omega.h>
#include <omega/api_libc.h>
#if OMG_SUPPORT_WIN
#include <omega/omega_win.h>
#endif
#if OMG_SUPPORT_SDL2
#include <omega/omega_sdl2.h>
#endif
#if OMG_HAS_STD
#include <string.h>
#include <stdlib.h>
#endif
#include <math.h>
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

#if OMG_IMPL_PEI386_RUNTIME_RELOCATOR
void _pei386_runtime_relocator(void) {
	static bool s_relocated = false;
	if(s_relocated) {
		return;
	}
	s_relocated = true;
}
#endif

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

int omg_std_strcmp(const char* str1, const char* str2) {
    int result;
    while (1) {
        result = ((unsigned char)*str1 - (unsigned char)*str2);
        if (result != 0 || (*str1 == '\0')) {
            break;
        }
        ++str1;
        ++str2;
    }
    return result;
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

double omg_std_floor(double x) {
    // Should I check bounds?
    long long n = (long long)x;
    double d = (double)n;
    if (d == x || x >= 0)
        return d;
    else
        return d - 1;
}

double omg_std_ceil(double x) {
    double integer = omg_def_std->floor(x);
    double fraction = x - integer;
    if (fraction > 0.0) {
        integer += 1.0;
    }
    return integer;
}

double omg_std_round(double x) {
    if (x >= 0.0) {
        return omg_def_std->floor(x + 0.5);
    } else {
        return omg_def_std->ceil(x - 0.5);
    }
}

float omg_std_floorf(float x) {
    return (float)omg_def_std->floor((double)x);
}

float omg_std_ceilf(float x) {
    return (float)omg_def_std->ceil((double)x);
}

float omg_std_roundf(float x) {
    return (float)omg_def_std->round((double)x);
}

long omg_std_lrint(double x) {
    return (long)omg_def_std->round(x);
}

void omg_std_qsort(void* base, size_t num, size_t size, int (*compare) (const void*, const void*)) {
    OMG_UNUSED(base, num, size, compare); // TODO
}

int omg_std_abs(int x) {
    return (x > 0) ? x : (-x);
}

double omg_std_fabs(double x) {
    return (x > 0.0) ? x : (-x);
}

float omg_std_fabsf(float x) {
    return (x > 0.0f) ? x : (-x);
}

double omg_std_pow(double base, double exponent) {
    double res = 1.0;
    for (double i = 0.0; i < exponent; i++) {
        res *= base;
    }
    return res;
}

float omg_std_powf(float base, float exponent) {
    float res = 1.0f;
    for (float i = 0.0f; i < exponent; i++) {
        res *= base;
    }
    return res;
}

OMG_Std* omg_std_get_default_handle(void) {
    return omg_def_std;
}

void omg_std_set_default_handle(OMG_Std* this) {
    omg_def_std = this;
}

double omg_dummy_double_func(double x) {
    return x;
}

double omg_dummy_double2_func(double x, double y) {
    return x / y;
}

float omg_dummy_float_func(float x) {
    return x;
}

float omg_dummy_float2_func(float x, float y) {
    return x / y;
}

double omg_std_atan2(double x, double y) {
    return omg_def_std->atan(x / y);
}

float omg_std_atan2f(float x, float y) {
    return (float)omg_def_std->atan2((double)x, (double)y);
}

double omg_std_fmod(double x, double y) {
    // TODO: trunc function
    return x - ((double)((int32_t)(x / y))) * y;
}

float omg_std_fmodf(float x, float y) {
    return (float)omg_def_std->fmod((double)x, (double)y);
}

float omg_std_sinf(float x) {
    return (float)omg_def_std->sin((double)x);
}

float omg_std_cosf(float x) {
    return (float)omg_def_std->cos((double)x);
}

float omg_std_tanf(float x) {
    return (float)omg_def_std->tan((double)x);
}

float omg_std_atanf(float x) {
    return (float)omg_def_std->atan((double)x);
}

float omg_std_sqrtf(float x) {
    return (float)omg_def_std->sqrt((double)x);
}

void omg_std_fill_defaults(OMG_Std* this) {
    this->utf8strlen = omg_std_utf8strlen;
    this->utf8strnlen = omg_std_utf8strnlen;
#if OMG_HAS_STD
    // Lol everyone should have them
    this->memset = memset;
    this->memcpy = memcpy;
    this->memmove = memmove;
    this->memcmp = memcmp;
    this->strlen = strlen;
    this->strcmp = strcmp;
    this->strnlen = strnlen;
#else
    this->memset = omg_std_memset;
    this->memcpy = omg_std_memcpy;
    this->memmove = omg_std_memmove;
    this->memcmp = omg_std_memcmp;
    this->strcmp = omg_std_strcmp;
    this->strlen = omg_std_strlen;
    this->strnlen = omg_std_strnlen;
#endif
#if OMG_USE_LIBMATH
#if OMG_USE_LIBMATH_EXTRA
    this->round = round;
    this->floorf = floorf;
    this->ceilf = ceilf;
    this->roundf = roundf;
    this->atan2 = atan2;
    this->atan2f = atan2f;
    this->fmod = fmod;
    this->fmodf = fmodf;
    this->sinf = sinf;
    this->cosf = cosf;
    this->tanf = tanf;
    this->atanf = atanf;
    this->atan2f = atan2f;
    this->sqrtf = sqrtf;
    this->fabsf = fabsf;
    this->powf = powf;
#else
    this->round = omg_std_round;
    this->floorf = omg_std_floorf;
    this->ceilf = omg_std_ceilf;
    this->roundf = omg_std_roundf;
    this->atan2 = omg_std_atan2;
    this->atan2f = omg_std_atan2f;
    this->fmod = omg_std_fmod;
    this->fmodf = omg_std_fmodf;
    this->sinf = omg_std_sinf;
    this->cosf = omg_std_cosf;
    this->tanf = omg_std_tanf;
    this->atanf = omg_std_atanf;
    this->sqrtf = omg_std_sqrtf;
    this->fabsf = omg_std_fabsf;
    this->powf = omg_std_powf;
#endif
    this->floor = floor;
    this->ceil = ceil;
    this->sin = sin;
    this->cos = cos;
    this->tan = tan;
    this->atan = atan;
    this->sqrt = sqrt;
    this->abs = abs;
    this->fabs = fabs;
    this->pow = pow;
#else
    this->floor = omg_std_floor;
    this->ceil = omg_std_ceil;
    this->sin = omg_dummy_double_func;
    this->cos = omg_dummy_double_func;
    this->tan = omg_dummy_double_func;
    this->atan = omg_dummy_double_func;
    this->atan2 = omg_dummy_double2_func;
    this->sqrt = omg_dummy_double_func;
    this->fmod = omg_dummy_double2_func;
    this->sinf = omg_dummy_float_func;
    this->cosf = omg_dummy_float_func;
    this->tanf = omg_dummy_float_func;
    this->atanf = omg_dummy_float_func;
    this->atan2f = omg_dummy_float2_func;
    this->sqrtf = omg_dummy_float_func;
    this->fmodf = omg_dummy_float2_func;
    this->abs = omg_std_abs;
    this->fabs = omg_std_fabs;
    this->fabsf = omg_std_fabsf;
    this->pow = omg_std_pow;
    this->powf = omg_std_powf;
#endif
#if OMG_HAVE_QSORT
    this->qsort = qsort;
#else
    this->qsort = omg_std_qsort;
#endif
#if OMG_HAVE_LRINT && OMG_USE_LIBMATH
    this->lrint = lrint;
#else
    this->lrint = omg_std_lrint;
#endif
#if OMG_HAVE_WCSLEN
    this->wcslen = wcslen;
#else
    this->wcslen = omg_std_wcslen;
#endif
#if OMG_HAVE__STRREV
    this->strrev = _strrev;
#else
    this->strrev = omg_std_strrev;
#endif
#if OMG_HAVE_ITOA
    this->itoa = itoa;
#else
    this->itoa = omg_std_itoa;
#endif
#if OMG_HAVE__UITOA
    this->itoa = _uitoa;
#else
    this->uitoa = omg_std_uitoa;
#endif
#if OMG_HAVE_LTOA
    this->ltoa = ltoa;
#else
    this->ltoa = omg_std_ltoa;
#endif
#if OMG_HAVE__ULTOA
    this->ultoa = _ultoa;
#else
    this->ultoa = omg_std_ultoa;
#endif
#if OMG_HAVE__I64TOA
    this->lltoa = _i64toa;
#else
    this->lltoa = omg_std_lltoa;
#endif
#if OMG_HAVE__I64TOA
    this->ulltoa = _ui64toa;
#else
    this->ulltoa = omg_std_ulltoa;
#endif
#if OMG_SUPPORT_LIBC
    OMG_Omega* omg = omg_get_default_omega();
    this->sleep = OMG_ISNULL(omg->libc) ? NULL : ((OMG_Libc*)omg->libc)->sleep;
#else
    this->sleep = NULL;
#endif
}

bool omg_std_unicode_char_to_utf8(char* out, uint32_t code) {
    // https://gist.github.com/MightyPork/52eda3e5677b4b03524e40c9f0ab1da5
    if (code <= 0x7F) {
        out[0] = (char) code;
        out[1] = 0;
        return false;
    }
    else if (code <= 0x07FF) {
        out[0] = (char) (((code >> 6) & 0x1F) | 0xC0);
        out[1] = (char) (((code >> 0) & 0x3F) | 0x80);
        out[2] = 0;
        return false;
    }
    else if (code <= 0xFFFF) {
        out[0] = (char) (((code >> 12) & 0x0F) | 0xE0);
        out[1] = (char) (((code >>  6) & 0x3F) | 0x80);
        out[2] = (char) (((code >>  0) & 0x3F) | 0x80);
        out[3] = 0;
        return false;
    }
    else if (code <= 0x10FFFF) {
        out[0] = (char) (((code >> 18) & 0x07) | 0xF0);
        out[1] = (char) (((code >> 12) & 0x3F) | 0x80);
        out[2] = (char) (((code >>  6) & 0x3F) | 0x80);
        out[3] = (char) (((code >>  0) & 0x3F) | 0x80);
        out[4] = 0;
        return false;
    }
    else { 
        out[0] = (char) 0xEF;  
        out[1] = (char) 0xBF;
        out[2] = (char) 0xBD;
        out[3] = 0;
        return true;
    }
}

// Thanks to SDL2
bool omg_std_utf32_char_to_utf8(uint32_t codepoint, char* text) {
    if (codepoint <= 0x7F) {
        text[0] = (char)codepoint;
        text[1] = '\0';
    } else if (codepoint <= 0x7FF) {
        text[0] = 0xC0 | (char)((codepoint >> 6) & 0x1F);
        text[1] = 0x80 | (char)(codepoint & 0x3F);
        text[2] = '\0';
    } else if (codepoint <= 0xFFFF) {
        text[0] = 0xE0 | (char)((codepoint >> 12) & 0x0F);
        text[1] = 0x80 | (char)((codepoint >> 6) & 0x3F);
        text[2] = 0x80 | (char)(codepoint & 0x3F);
        text[3] = '\0';
    } else if (codepoint <= 0x10FFFF) {
        text[0] = 0xF0 | (char)((codepoint >> 18) & 0x0F);
        text[1] = 0x80 | (char)((codepoint >> 12) & 0x3F);
        text[2] = 0x80 | (char)((codepoint >> 6) & 0x3F);
        text[3] = 0x80 | (char)(codepoint & 0x3F);
        text[4] = '\0';
    } else {
        return true;
    }
    return false;
}

bool omg_std_win_utf16_char_to_utf8(uint32_t high_surrogate, uint32_t low_surrogate, char* text) {
    const uint32_t SURROGATE_OFFSET = 0x10000 - (0xD800 << 10) - 0xDC00;
    const uint32_t codepoint = (high_surrogate << 10) + low_surrogate + SURROGATE_OFFSET;
    return omg_std_utf32_char_to_utf8(codepoint, text);
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

bool omg_string_add_bool(OMG_String* this, const bool bool_to_add) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (omg_string_ensure_free_len(this, 6))
        return true;
    if (bool_to_add) {
        this->ptr[this->len] = 'T';
        this->ptr[this->len + 1] = 'r';
        this->ptr[this->len + 2] = 'u';
        this->ptr[this->len + 3] = 'e';
        this->len += 4;
    }
    else {
        this->ptr[this->len] = 'F';
        this->ptr[this->len + 1] = 'a';
        this->ptr[this->len + 2] = 'l';
        this->ptr[this->len + 3] = 's';
        this->ptr[this->len + 4] = 'e';
        this->len += 5;
    }
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
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (OMG_ISNULL(str_to_add))
        return omg_string_add(this, &OMG_STRING_MAKE_STATIC(OMG_NULL_STR));
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

bool omg_string_add_point(OMG_String* this, const OMG_Point* point_to_add) {
    if (OMG_ISNULL(point_to_add))
        return omg_string_add(this, &OMG_STRING_MAKE_STATIC(OMG_NULL_STR));
    return OMG_ISNULL(point_to_add) || omg_string_add_int(this, point_to_add->x) ||
        omg_string_add_char(this, 'x') || omg_string_add_int(this, point_to_add->y);
}

bool omg_string_add_fpoint(OMG_String* this, const OMG_FPoint* fpoint_to_add) {
    if (OMG_ISNULL(fpoint_to_add))
        return omg_string_add(this, &OMG_STRING_MAKE_STATIC(OMG_NULL_STR));
    return OMG_ISNULL(fpoint_to_add) || omg_string_add_float(this, fpoint_to_add->x) ||
        omg_string_add_char(this, 'x') || omg_string_add_float(this, fpoint_to_add->y);
}

bool omg_string_add_dpoint(OMG_String* this, const OMG_DPoint* dpoint_to_add) {
    if (OMG_ISNULL(dpoint_to_add))
        return omg_string_add(this, &OMG_STRING_MAKE_STATIC(OMG_NULL_STR));
    return OMG_ISNULL(dpoint_to_add) || omg_string_add_double(this, dpoint_to_add->x) ||
        omg_string_add_char(this, 'x') || omg_string_add_double(this, dpoint_to_add->y);
}

bool omg_string_add_rect(OMG_String* this, const OMG_Rect* rect_to_add) {
    if (OMG_ISNULL(rect_to_add))
        return omg_string_add(this, &OMG_STRING_MAKE_STATIC(OMG_NULL_STR));
    return OMG_ISNULL(rect_to_add) || omg_string_add_int(this, rect_to_add->x) ||
        omg_string_add_char(this, 'x') || omg_string_add_int(this, rect_to_add->y) ||
        omg_string_add_char(this, ';') || omg_string_add_int(this, rect_to_add->w) ||
        omg_string_add_char(this, 'x') || omg_string_add_int(this, rect_to_add->h);
}

bool omg_string_add_frect(OMG_String* this, const OMG_FRect* frect_to_add) {
    if (OMG_ISNULL(frect_to_add))
        return omg_string_add(this, &OMG_STRING_MAKE_STATIC(OMG_NULL_STR));
    return OMG_ISNULL(frect_to_add) || omg_string_add_float(this, frect_to_add->x) ||
        omg_string_add_char(this, 'x') || omg_string_add_float(this, frect_to_add->y) ||
        omg_string_add_char(this, ';') || omg_string_add_float(this, frect_to_add->w) ||
        omg_string_add_char(this, 'x') || omg_string_add_float(this, frect_to_add->h);
}

bool omg_string_add_drect(OMG_String* this, const OMG_DRect* drect_to_add) {
    if (OMG_ISNULL(drect_to_add))
        return omg_string_add(this, &OMG_STRING_MAKE_STATIC(OMG_NULL_STR));
    return OMG_ISNULL(drect_to_add) || omg_string_add_double(this, drect_to_add->x) ||
        omg_string_add_char(this, 'x') || omg_string_add_double(this, drect_to_add->y) ||
        omg_string_add_char(this, ';') || omg_string_add_double(this, drect_to_add->w) ||
        omg_string_add_char(this, 'x') || omg_string_add_double(this, drect_to_add->h);
}

bool omg_string_add_wchar_p(OMG_String* this, const wchar_t* wstr_to_add) {
    if (this->type < OMG_STRING_BUFFER)
        return true;
    if (OMG_ISNULL(wstr_to_add))
        return omg_string_add(this, &OMG_STRING_MAKE_STATIC(OMG_NULL_STR));
    OMG_Omega* tmp_omg = omg_get_default_omega();
    if (OMG_ISNULL(tmp_omg))
        return true;
    int omg_type = tmp_omg->type;
#if OMG_SUPPORT_SDL2
    if (omg_type == OMG_OMEGA_TYPE_SDL2) {
        OMG_OmegaSdl2* omg = (OMG_OmegaSdl2*)tmp_omg;
        char* out_buf = omg->sdl2->SDL_iconv_string(
            "UTF-8", "WCHAR_T",
            (char*)wstr_to_add, (omg->sdl2->SDL_wcslen(wstr_to_add) + 1) * sizeof(wchar_t)
        );
        if (OMG_ISNULL(out_buf))
            return true;
        size_t buf_len = omg_def_std->strlen(out_buf);
        if (buf_len == 0)
            return true;
        bool res = omg_string_add_char_p(this, out_buf);
        omg->sdl2->SDL_free(out_buf);
        return res;
    }
#endif
#if OMG_SUPPORT_WIN
    if ((omg_type == OMG_OMEGA_TYPE_WIN) || true) {
        size_t need_len = omg_def_std->wcslen(wstr_to_add);
        size_t need_count;
        _OMG_WIN_GET_DECODE_SIZE(need_count, wstr_to_add, tmp_omg->k32, need_len);
        if ((need_count == 0) || omg_string_ensure_free_len(this, need_count))
            return true;
        int res = ((OMG_Kernel32*)(tmp_omg->k32))->WideCharToMultiByte(CP_UTF8, 0, wstr_to_add, (int)need_len, this->ptr + this->len, (int)need_count, NULL, NULL);
        if (res > 0) {
            this->len += (size_t)res;
            return false;
        }
        return true;
    }
#endif
    OMG_UNUSED(this, wstr_to_add);
    return true;
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
    if (OMG_ISNULL(new_str))
        return omg_string_add(this, &OMG_STRING_MAKE_STATIC(OMG_NULL_STR));
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
    if (OMG_ISNULL(this))
        return false;
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

bool omg_string_equal(const OMG_String* this, const OMG_String* other_string) {
    if (this == other_string)
        return true;
    if (OMG_ISNULL(this) || OMG_ISNULL(other_string))
        return false;
    if (this->len != other_string->len)
        return false;
    if (this->ptr == other_string->ptr)
        return true;
    return !omg_def_std->memcmp(this->ptr, other_string->ptr, this->len);
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

OMG_String* omg_dummy_string_create(void) {
    static OMG_String dummy_str = {
        .len = 0,
        .ptr = "\0",
        .size = 1,
        .type = -1
    };
    return &dummy_str;
}

static char b64_encoding_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};
static int b64_mod_table[] = { 0, 2, 1 };

OMG_String* omg_base64_encode(OMG_String* input_str, OMG_String* output_str) {
    // https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
    if (OMG_ISNULL(input_str) || input_str->len <= 1)
        return omg_dummy_string_create();
    size_t out_size = OMG_B64_ENCODED_LEN(input_str->len);
    if (OMG_ISNULL(output_str)) {
        static OMG_String temp_str;
        output_str = &temp_str;
        if (omg_string_init_dynamic(output_str, NULL) || omg_string_resize(output_str, out_size))
            return omg_dummy_string_create();
    }
    else if (omg_string_ensure_free_len(output_str, out_size))
        return omg_dummy_string_create();
    uint32_t octet_a, octet_b, octet_c, triple;
    for (size_t i = 0, j = output_str->len; i < input_str->len;) {
        octet_a = i < input_str->len ? (unsigned char)input_str->ptr[i++] : 0;
        octet_b = i < input_str->len ? (unsigned char)input_str->ptr[i++] : 0;
        octet_c = i < input_str->len ? (unsigned char)input_str->ptr[i++] : 0;
        triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
        output_str->ptr[j++] = b64_encoding_table[(triple >> 3 * 6) & 0x3F];
        output_str->ptr[j++] = b64_encoding_table[(triple >> 2 * 6) & 0x3F];
        output_str->ptr[j++] = b64_encoding_table[(triple >> 1 * 6) & 0x3F];
        output_str->ptr[j++] = b64_encoding_table[(triple >> 0 * 6) & 0x3F];
    }
    for (size_t i = 0; i < (size_t)b64_mod_table[input_str->len % 3]; i++)
        output_str->ptr[output_str->len + out_size - 1 - i] = '=';
    output_str->len += out_size;
    return output_str;
}
