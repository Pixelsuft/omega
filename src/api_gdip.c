#include <omega/api_gdip.h>

#if OMG_IS_WIN && OMG_SUPPORT_GDIP
#ifdef _MSC_VER
#define OMG_PROC_ADDRESS (void*)GetProcAddress
#else
#define OMG_PROC_ADDRESS GetProcAddress
#endif
#define LOAD_SYSTEM_LIBRARY(lib_path) LoadLibraryExW(lib_path, NULL, LOAD_IGNORE_CODE_AUTHZ_LEVEL | LOAD_LIBRARY_SEARCH_SYSTEM32)
#if OMG_WINAPI_DYNAMIC
#define LOAD_REQUIRED(func_name) this->func_name = OMG_PROC_ADDRESS(this->handle, #func_name)
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#endif

bool omg_winapi_gdip_load(OMG_Gdip* this) {
#if OMG_WINAPI_DYNAMIC
    this->handle = LOAD_SYSTEM_LIBRARY(L"GdiPlus.dll");
    if (OMG_ISNULL(this->handle))
        return true;
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED(GdiplusStartup);
    LOAD_REQUIRED(GdiplusShutdown);
    LOAD_REQUIRED(GdipGetImageDecodersSize);
    LOAD_REQUIRED(GdipGetImageDecoders);
    LOAD_REQUIRED(GdipGetImageEncodersSize);
    LOAD_REQUIRED(GdipGetImageEncoders);
    LOAD_REQUIRED(GdipBitmapLockBits);
    LOAD_REQUIRED(GdipBitmapUnlockBits);
    LOAD_REQUIRED(GdipGetImageWidth);
    LOAD_REQUIRED(GdipGetImageHeight);
    LOAD_REQUIRED(GdipGetImagePixelFormat);
    LOAD_REQUIRED(GdipLoadImageFromStream);
    LOAD_REQUIRED(GdipLoadImageFromFile);
    LOAD_REQUIRED(GdipCreateBitmapFromStreamICM);
    LOAD_REQUIRED(GdipCreateBitmapFromFileICM);
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_winapi_gdip_free(OMG_Gdip* this) {
#if OMG_WINAPI_DYNAMIC
    if (OMG_ISNULL(this->handle))
        return true;
    return (bool)FreeLibrary(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}
#endif
