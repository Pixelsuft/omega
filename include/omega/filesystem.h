#pragma once
#include <omega/ostd.h>
#define _OMG_FILE_MODE_TO_STD(mode, str_mode) do { \
    if (mode == OMG_FILE_MODE_R) \
        str_mode = "r"; \
    else if (mode == OMG_FILE_MODE_W) \
        str_mode = "w"; \
    else if (mode == OMG_FILE_MODE_A) \
        str_mode = "a"; \
    else if (mode == OMG_FILE_MODE_RP) \
        str_mode = "r+"; \
    else if (mode == OMG_FILE_MODE_WP) \
        str_mode = "w+"; \
    else if (mode == OMG_FILE_MODE_AP) \
        str_mode = "a+"; \
    else if (mode == OMG_FILE_MODE_RB) \
        str_mode = "rb"; \
    else if (mode == OMG_FILE_MODE_WB) \
        str_mode = "wb"; \
    else if (mode == OMG_FILE_MODE_AB) \
        str_mode = "ab"; \
    else if (mode == OMG_FILE_MODE_RPB) \
        str_mode = "r+b"; \
    else if (mode == OMG_FILE_MODE_WPB) \
        str_mode = "w+b"; \
    else if (mode == OMG_FILE_MODE_APB) \
        str_mode = "a+b"; \
    else if (mode == OMG_FILE_MODE_RT) \
        str_mode = "rt"; \
    else if (mode == OMG_FILE_MODE_WT) \
        str_mode = "wt"; \
    else if (mode == OMG_FILE_MODE_AT) \
        str_mode = "at"; \
    else if (mode == OMG_FILE_MODE_RPT) \
        str_mode = "r+t"; \
    else if (mode == OMG_FILE_MODE_WPT) \
        str_mode = "w+t"; \
    else if (mode == OMG_FILE_MODE_APT) \
        str_mode = "a+t"; \
} while (0)

#define OMG_FILE_MODE_R 1
#define OMG_FILE_MODE_W 2
#define OMG_FILE_MODE_A 3
#define OMG_FILE_MODE_RP 4
#define OMG_FILE_MODE_WP 5
#define OMG_FILE_MODE_AP 6
#define OMG_FILE_MODE_RB 7
#define OMG_FILE_MODE_WB 8
#define OMG_FILE_MODE_AB 9
#define OMG_FILE_MODE_RPB 10
#define OMG_FILE_MODE_WPB 11
#define OMG_FILE_MODE_APB 12
#define OMG_FILE_MODE_RT 13
#define OMG_FILE_MODE_WT 14
#define OMG_FILE_MODE_AT 15
#define OMG_FILE_MODE_RPT 16
#define OMG_FILE_MODE_WPT 17
#define OMG_FILE_MODE_APT 18

#define OMG_FILE_SEEK_SET 0
#define OMG_FILE_SEEK_CUR 1
#define OMG_FILE_SEEK_END 2

#define OMG_FILE_TYPE_UNKNOWN 0
#define OMG_FILE_TYPE_WINFILE 1
#define OMG_FILE_TYPE_STDFILE 2
#define OMG_FILE_TYPE_JNIFILE 3
#define OMG_FILE_TYPE_MEMORY 4
#define OMG_FILE_TYPE_MEMORY_RO 5

#if OMG_IS_WIN
#define OMG_PATH_DELIM '\\'
#else
#define OMG_PATH_DELIM '/'
#endif

typedef struct OMG_File {
    OMG_String fp;
    /* Close file and free mem */
    bool (*destroy)(struct OMG_File* file);
    /* Get file size */
    int64_t (*get_size)(struct OMG_File* file);
    /* Set pos */
    int64_t (*seek)(struct OMG_File* file, int64_t offset, int whence);
    /* Get pos */
    int64_t (*tell)(struct OMG_File* file);
    /* Read */
    size_t (*read)(struct OMG_File* file, void* buf, size_t size, size_t maxnum);
    /* Write */
    size_t (*write)(struct OMG_File* file, const void* buf, size_t size, size_t num);
    void* extra1;
    void* omg;
    int mode;
    int type;
    bool was_allocated;
} OMG_File;

typedef struct {
    OMG_File parent;
    uint8_t* base;
    uint8_t* here;
    uint8_t* stop;
} OMG_FileMem;

#if OMG_HAS_STD
typedef struct {
    OMG_File parent;
    void* file;
} OMG_FileStd;
#endif
