#pragma once
#include <omega/config.h>
#include <omega/entry.h>
#include <omega/memory.h>

typedef struct OMG_Omega {
    OMG_Memory* mem;
    bool (*destroy)(struct OMG_Omega* this);
    bool should_free_mem;
} OMG_Omega;

OMG_API OMG_Omega* omg_create(OMG_EntryData* data);
OMG_API bool omg_init(OMG_Omega* this);
