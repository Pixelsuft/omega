#pragma once
#include <omega/config.h>
#include <omega/ostd.h>

// Should I keep buf?
typedef struct {
    size_t size;
    size_t len;
    int type;
    const void* data;
} OMG_ArrayBase;

#define OMG_Array(array_type) struct { \
    size_t size; \
    size_t len; \
    int type; \
    array_type* data; \
}
