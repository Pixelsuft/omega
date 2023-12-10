#pragma once
#include <omega/config.h>
#include <omega/ostd.h>

OMG_API void* omg_static_lib_load(const OMG_String* fn, const wchar_t* adv_fn);
OMG_API void* omg_static_lib_func(void* lib, const OMG_String* func_name);
OMG_API bool omg_static_lib_free(void* lib);
