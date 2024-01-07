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

#define OMG_FILE_SEEK_SET 0
#define OMG_FILE_SEEK_CUR 1
#define OMG_FILE_SEEK_END 2

typedef struct OMG_File {
    bool (*destroy)(struct OMG_File* file);
    int64_t (*get_size)(struct OMG_File* file);
    int64_t (*seek)(struct OMG_File* file, int64_t offset, int whence);
    int64_t (*tell)(struct OMG_File* file);
    size_t (*read)(struct OMG_File* file, void* buf, size_t size, size_t maxnum);
    void* extra1;
    void* omg;
    const OMG_String* fp;
    int mode;
    bool was_allocated;
} OMG_File;
