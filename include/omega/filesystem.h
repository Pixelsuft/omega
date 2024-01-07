#pragma once
#include <omega/ostd.h>
#define _OMG_FILE_MODE_TO_STD(mode, str_mode) do { \
    if (mode == OMG_FILE_MODE_R) \
        str_mode = "r"; \
} while (0)

#define OMG_FILE_MODE_R 1

typedef struct OMG_File {
    bool (*destroy)(struct OMG_File* file);
    int64_t (*get_size)(struct OMG_File* file);
    void* extra1;
    void* omg;
    const OMG_String* fp;
    int mode;
    bool was_allocated;
} OMG_File;
