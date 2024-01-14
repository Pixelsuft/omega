#include <omega/api_spng.h>

#if OMG_SUPPORT_SPNG
#if OMG_SPNG_DYNAMIC
#include <omega/api_static.h>
#define LOAD_REQUIRED(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#endif

OMG_API bool omg_spng_dll_load(OMG_Spng* this, const OMG_String* dll_path) {
#if OMG_SPNG_DYNAMIC
    if (OMG_ISNULL(dll_path))
#if OMG_IS_WIN
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("spng.dll"), L"spng.dll");
#else
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("libspng.so"), NULL);
#endif
    else
        this->handle = omg_static_lib_load(dll_path, NULL);
    if (OMG_ISNULL(this->handle))
        return true;
#else
    OMG_UNUSED(dll_path);
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED(spng_ctx_new);
    LOAD_REQUIRED(spng_ctx_new2);
    LOAD_REQUIRED(spng_ctx_free);
    LOAD_REQUIRED(spng_set_png_buffer);
    LOAD_REQUIRED(spng_decoded_image_size);
    LOAD_REQUIRED(spng_decode_image);
    LOAD_REQUIRED(spng_get_ihdr);
    LOAD_REQUIRED(spng_strerror);
    OMG_END_POINTER_CAST();
    return false;
}

OMG_API bool omg_spng_dll_free(OMG_Spng* this) {
#if OMG_SPNG_DYNAMIC
    return omg_static_lib_free(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}
#endif
