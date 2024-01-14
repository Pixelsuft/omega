#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_RAYLIB
#include <omega/ostd.h>
#if !OMG_SPNG_DYMANIC
#include <spng.h>
#define OMG_SPNG_STD_PREFIX SPNG_API
#else
#define OMG_SPNG_STD_PREFIX

enum spng_errno {
    SPNG_IO_ERROR = -2,
    SPNG_IO_EOF = -1,
    SPNG_OK = 0,
    SPNG_EINVAL,
    SPNG_EMEM,
    SPNG_EOVERFLOW,
    SPNG_ESIGNATURE,
    SPNG_EWIDTH,
    SPNG_EHEIGHT,
    SPNG_EUSER_WIDTH,
    SPNG_EUSER_HEIGHT,
    SPNG_EBIT_DEPTH,
    SPNG_ECOLOR_TYPE,
    SPNG_ECOMPRESSION_METHOD,
    SPNG_EFILTER_METHOD,
    SPNG_EINTERLACE_METHOD,
    SPNG_EIHDR_SIZE,
    SPNG_ENOIHDR,
    SPNG_ECHUNK_POS,
    SPNG_ECHUNK_SIZE,
    SPNG_ECHUNK_CRC,
    SPNG_ECHUNK_TYPE,
    SPNG_ECHUNK_UNKNOWN_CRITICAL,
    SPNG_EDUP_PLTE,
    SPNG_EDUP_CHRM,
    SPNG_EDUP_GAMA,
    SPNG_EDUP_ICCP,
    SPNG_EDUP_SBIT,
    SPNG_EDUP_SRGB,
    SPNG_EDUP_BKGD,
    SPNG_EDUP_HIST,
    SPNG_EDUP_TRNS,
    SPNG_EDUP_PHYS,
    SPNG_EDUP_TIME,
    SPNG_EDUP_OFFS,
    SPNG_EDUP_EXIF,
    SPNG_ECHRM,
    SPNG_EPLTE_IDX,
    SPNG_ETRNS_COLOR_TYPE,
    SPNG_ETRNS_NO_PLTE,
    SPNG_EGAMA,
    SPNG_EICCP_NAME,
    SPNG_EICCP_COMPRESSION_METHOD,
    SPNG_ESBIT,
    SPNG_ESRGB,
    SPNG_ETEXT,
    SPNG_ETEXT_KEYWORD,
    SPNG_EZTXT,
    SPNG_EZTXT_COMPRESSION_METHOD,
    SPNG_EITXT,
    SPNG_EITXT_COMPRESSION_FLAG,
    SPNG_EITXT_COMPRESSION_METHOD,
    SPNG_EITXT_LANG_TAG,
    SPNG_EITXT_TRANSLATED_KEY,
    SPNG_EBKGD_NO_PLTE,
    SPNG_EBKGD_PLTE_IDX,
    SPNG_EHIST_NO_PLTE,
    SPNG_EPHYS,
    SPNG_ESPLT_NAME,
    SPNG_ESPLT_DUP_NAME,
    SPNG_ESPLT_DEPTH,
    SPNG_ETIME,
    SPNG_EOFFS,
    SPNG_EEXIF,
    SPNG_EIDAT_TOO_SHORT,
    SPNG_EIDAT_STREAM,
    SPNG_EZLIB,
    SPNG_EFILTER,
    SPNG_EBUFSIZ,
    SPNG_EIO,
    SPNG_EOF,
    SPNG_EBUF_SET,
    SPNG_EBADSTATE,
    SPNG_EFMT,
    SPNG_EFLAGS,
    SPNG_ECHUNKAVAIL,
    SPNG_ENCODE_ONLY,
    SPNG_EOI,
    SPNG_ENOPLTE,
    SPNG_ECHUNK_LIMITS,
    SPNG_EZLIB_INIT,
    SPNG_ECHUNK_STDLEN,
    SPNG_EINTERNAL,
    SPNG_ECTXTYPE,
    SPNG_ENOSRC,
    SPNG_ENODST,
    SPNG_EOPSTATE,
    SPNG_ENOTFINAL,
};

enum spng_format {
    SPNG_FMT_RGBA8 = 1,
    SPNG_FMT_RGBA16 = 2,
    SPNG_FMT_RGB8 = 4,
    SPNG_FMT_GA8 = 16,
    SPNG_FMT_GA16 = 32,
    SPNG_FMT_G8 = 64,
    SPNG_FMT_PNG = 256,
    SPNG_FMT_RAW = 512
};

struct spng_ihdr {
    uint32_t width;
    uint32_t height;
    uint8_t bit_depth;
    uint8_t color_type;
    uint8_t compression_method;
    uint8_t filter_method;
    uint8_t interlace_method;
};

typedef void* spng_malloc_fn(size_t);
typedef void* spng_realloc_fn(void*, size_t);
typedef void* spng_calloc_fn(size_t count, size_t);
typedef void spng_free_fn(void*);

struct spng_alloc {
    spng_malloc_fn *malloc_fn;
    spng_realloc_fn *realloc_fn;
    spng_calloc_fn *calloc_fn;
    spng_free_fn *free_fn;
};

typedef struct spng_ctx;
#endif

typedef struct {
    void* handle;
    spng_ctx* OMG_SPNG_STD_PREFIX (*spng_ctx_new)(int);
    spng_ctx* OMG_SPNG_STD_PREFIX (*spng_ctx_new2)(struct spng_alloc*, int);
    void OMG_SPNG_STD_PREFIX (*spng_ctx_free)(spng_ctx*);
    int OMG_SPNG_STD_PREFIX (*spng_set_png_buffer)(spng_ctx*, const void*, size_t);
    int OMG_SPNG_STD_PREFIX (*spng_decoded_image_size)(spng_ctx*, int, size_t*);
    int OMG_SPNG_STD_PREFIX (*spng_decode_image)(spng_ctx*, void*, size_t, int, int);
    int OMG_SPNG_STD_PREFIX (*spng_get_ihdr)(spng_ctx*, struct spng_ihdr*);
} OMG_Spng;
#endif
