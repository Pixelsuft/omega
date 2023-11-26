#pragma once
#include <omega/config.h>
#include <omega/entry.h>
#include <omega/memory.h>
#include <omega/ostd.h>

#define OMG_LOG_INFO 1
#define OMG_LOG_WARN 2
#define OMG_LOG_ERROR 3
#define OMG_LOG_FATAL 4
#define OMG_LOG_DISABLED 5

typedef struct OMG_Omega {
    OMG_Memory* mem;
    OMG_Std* std;
    void* unused;
    bool (*destroy)(struct OMG_Omega* this);
    void (*log_info_str)(struct OMG_Omega* this, const char* data, size_t size);
    int log_level;
    bool should_free_mem;
    bool should_free_std;
} OMG_Omega;

OMG_API OMG_Omega* omg_create(OMG_EntryData* data);
OMG_API bool omg_init(OMG_Omega* this);
