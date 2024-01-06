#pragma once
#include <omega/ostd.h>

#define OMG_FILE_MODE_R 1

typedef struct OMG_File {
    bool (*destroy)(struct OMG_File* file);
    void* extra1;
    void* omg;
    const OMG_String* fp;
    int mode;
    bool was_allocated;
} OMG_File;
