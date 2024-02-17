#include <omega/api_libc.h>

#if OMG_SUPPORT_LIBC
#if OMG_LIBC_DYNAMIC
#include <omega/api_static.h>
#define LOAD_REQUIRED(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#endif

bool omg_libc_dll_load(OMG_Libc* this, const OMG_String* dll_path) {
    this->loaded = false;
#if OMG_LIBC_DYNAMIC
    this->handle = omg_static_lib_load(dll_path, NULL);
    if (OMG_ISNULL(this->handle))
        return true;
#else
    OMG_UNUSED(dll_path);
#endif
    OMG_BEGIN_POINTER_CAST();
    // TODO
    OMG_END_POINTER_CAST();
    this->loaded = true;
    return false;
}

bool omg_libc_dll_free(OMG_Libc* this) {
    this->loaded = false;
#if OMG_LIBC_DYNAMIC
    return omg_static_lib_free(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}
#endif
