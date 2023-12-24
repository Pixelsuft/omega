#include <omega/api_raylib.h>
#if OMG_SUPPORT_RAYLIB
#include <omega/ostd.h>
#if OMG_RAYLIB_DYNAMIC
#include <omega/api_static.h>
#define LOAD_REQUIRED(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#endif

bool omg_raylib_dll_load(OMG_Raylib* this, const OMG_String* dll_path) {
#if OMG_RAYLIB_DYNAMIC
    if (OMG_ISNULL(dll_path))
#if OMG_IS_WIN
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("raylib.dll"), L"raylib.dll");
#else
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("libraylib.so"), NULL);
#endif
    else
        this->handle = omg_static_lib_load(dll_path, NULL);
    if (OMG_ISNULL(this->handle))
        return true;
#else
    OMG_UNUSED(dll_path);
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED(InitWindow);
    LOAD_REQUIRED(CloseWindow);
    LOAD_REQUIRED(WindowShouldClose);
    LOAD_REQUIRED(IsWindowReady);
    LOAD_REQUIRED(IsWindowState);
    LOAD_REQUIRED(SetWindowState);
    LOAD_REQUIRED(SetWindowTitle);
    LOAD_REQUIRED(SetConfigFlags);
    LOAD_REQUIRED(ClearWindowState);
    LOAD_REQUIRED(EnableEventWaiting);
    LOAD_REQUIRED(DisableEventWaiting);
    LOAD_REQUIRED(TraceLog);
    LOAD_REQUIRED(SetTraceLogLevel);
    LOAD_REQUIRED(MemAlloc);
    LOAD_REQUIRED(MemRealloc);
    LOAD_REQUIRED(MemFree);
    LOAD_REQUIRED(SetTraceLogCallback);
    LOAD_REQUIRED(IsWindowResized);
    LOAD_REQUIRED(BeginDrawing);
    LOAD_REQUIRED(EndDrawing);
    LOAD_REQUIRED(ClearBackground);
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_raylib_dll_free(OMG_Raylib* this) {
#if OMG_RAYLIB_DYNAMIC
    return omg_static_lib_free(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}

#endif
