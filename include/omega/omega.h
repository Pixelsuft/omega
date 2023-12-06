#pragma once
#include <omega/config.h>
#include <omega/entry.h>
#include <omega/memory.h>
#include <omega/ostd.h>

#define OMG_OMEGA_TYPE_NONE 0
#define OMG_OMEGA_TYPE_WIN 1
#define OMG_OMEGA_TYPE_SDL2 1

#define OMG_LOG_CATEGORY_INFO 1
#define OMG_LOG_CATEGORY_WARN 2
#define OMG_LOG_CATEGORY_ERROR 3
#define OMG_LOG_CATEGORY_FATAL 4
#define OMG_LOG_CATEGORY_DISABLED 5

typedef struct OMG_Omega {
    OMG_Memory* mem;
    OMG_Std* std;
    void* unused;
    bool (*default_init)(struct OMG_Omega* this);
    bool (*destroy)(struct OMG_Omega* this);
    void (*log_set_level)(struct OMG_Omega* this, const int log_level, const int omg_log_level);
    bool (*log_info_str)(struct OMG_Omega* this, const OMG_String* data);
    bool (*log_warn_str)(struct OMG_Omega* this, const OMG_String* data);
    bool (*log_error_str)(struct OMG_Omega* this, const OMG_String* data);
    bool (*log_fatal_str)(struct OMG_Omega* this, const OMG_String* data);
    int log_level_omg;
    int log_level;
    int type;
    bool should_free_mem;
    bool should_free_std;
} OMG_Omega;

OMG_API OMG_Omega* omg_create(OMG_EntryData* data);
OMG_API void omg_fill_on_create(OMG_Omega* this);
OMG_API OMG_Omega* omg_get_default_omega(void);
OMG_API bool omg_destroy(OMG_Omega* this);
OMG_API bool omg_init(OMG_Omega* this);
#if OMG_EXPORT_SHIT
OMG_API void omg_log_set_level(struct OMG_Omega* this, const int log_level, const int omg_log_level);
OMG_API bool omg_log_info_str(OMG_Omega* this, const OMG_String* data);
#endif
