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
    LOAD_REQUIRED(WaitTime);
    LOAD_REQUIRED(SetTargetFPS);
    LOAD_REQUIRED(GetFrameTime);
    LOAD_REQUIRED(GetFPS);
    LOAD_REQUIRED(SetTraceLogCallback);
    LOAD_REQUIRED(MaximizeWindow);
    LOAD_REQUIRED(MinimizeWindow);
    LOAD_REQUIRED(RestoreWindow);
    LOAD_REQUIRED(IsWindowResized);
    LOAD_REQUIRED(IsWindowFullscreen);
    LOAD_REQUIRED(IsWindowHidden);
    LOAD_REQUIRED(IsWindowMinimized);
    LOAD_REQUIRED(IsWindowMaximized);
    LOAD_REQUIRED(IsWindowFocused);
    LOAD_REQUIRED(ToggleFullscreen);
    LOAD_REQUIRED(ToggleBorderlessWindowed);
    LOAD_REQUIRED(GetScreenWidth);
    LOAD_REQUIRED(GetScreenHeight);
    LOAD_REQUIRED(GetRenderWidth);
    LOAD_REQUIRED(GetRenderHeight);
    LOAD_REQUIRED(SetWindowSize);
    LOAD_REQUIRED(SetWindowFocused);
    LOAD_REQUIRED(IsMouseButtonPressed);
    LOAD_REQUIRED(IsMouseButtonDown);
    LOAD_REQUIRED(IsMouseButtonReleased);
    LOAD_REQUIRED(IsMouseButtonUp);
    LOAD_REQUIRED(GetMouseX);
    LOAD_REQUIRED(GetMouseY);
    LOAD_REQUIRED(GetMousePosition);
    LOAD_REQUIRED(GetMouseDelta);
    LOAD_REQUIRED(SetMousePosition);
    LOAD_REQUIRED(SetMouseOffset);
    LOAD_REQUIRED(SetMouseScale);
    LOAD_REQUIRED(GetMouseWheelMove);
    LOAD_REQUIRED(GetMouseWheelMoveV);
    LOAD_REQUIRED(LoadImage);
    LOAD_REQUIRED(LoadImageRaw);
    LOAD_REQUIRED(IsImageReady);
    LOAD_REQUIRED(UnloadImage);
    LOAD_REQUIRED(LoadTexture);
    LOAD_REQUIRED(LoadTextureFromImage);
    LOAD_REQUIRED(LoadTextureCubemap);
    LOAD_REQUIRED(LoadRenderTexture);
    LOAD_REQUIRED(IsTextureReady);
    LOAD_REQUIRED(UnloadTexture);
    LOAD_REQUIRED(IsRenderTextureReady);
    LOAD_REQUIRED(UnloadRenderTexture);
    LOAD_REQUIRED(UpdateTexture);
    LOAD_REQUIRED(UpdateTextureRec);
    LOAD_REQUIRED(BeginDrawing);
    LOAD_REQUIRED(EndDrawing);
    LOAD_REQUIRED(BeginMode2D);
    LOAD_REQUIRED(EndMode2D);
    LOAD_REQUIRED(BeginTextureMode);
    LOAD_REQUIRED(EndTextureMode);
    LOAD_REQUIRED(BeginShaderMode);
    LOAD_REQUIRED(EndShaderMode);
    LOAD_REQUIRED(BeginBlendMode);
    LOAD_REQUIRED(EndBlendMode);
    LOAD_REQUIRED(BeginScissorMode);
    LOAD_REQUIRED(EndScissorMode);
    LOAD_REQUIRED(LoadShader);
    LOAD_REQUIRED(LoadShaderFromMemory);
    LOAD_REQUIRED(IsShaderReady);
    LOAD_REQUIRED(GetShaderLocation);
    LOAD_REQUIRED(GetShaderLocationAttrib);
    LOAD_REQUIRED(SetShaderValue);
    LOAD_REQUIRED(SetShaderValueV);
    LOAD_REQUIRED(SetShaderValueMatrix);
    LOAD_REQUIRED(SetShaderValueTexture);
    LOAD_REQUIRED(UnloadShader);
    LOAD_REQUIRED(ClearBackground);
    LOAD_REQUIRED(DrawTextureV);
    LOAD_REQUIRED(DrawTexturePro);
    LOAD_REQUIRED(DrawPixelV);
    LOAD_REQUIRED(DrawLineV);
    LOAD_REQUIRED(DrawLineEx);
    LOAD_REQUIRED(DrawCircleV);
    LOAD_REQUIRED(DrawEllipse);
    LOAD_REQUIRED(DrawEllipseLines);
    LOAD_REQUIRED(DrawCircleLinesV);
    LOAD_REQUIRED(DrawRectangleRec);
    LOAD_REQUIRED(DrawRectanglePro);
    LOAD_REQUIRED(DrawRectangleLinesEx);
    LOAD_REQUIRED(DrawRectangleRounded);
    LOAD_REQUIRED(DrawRectangleRoundedLines);
    LOAD_REQUIRED(DrawPoly);
    LOAD_REQUIRED(DrawPolyLines);
    LOAD_REQUIRED(DrawPolyLinesEx);
    LOAD_REQUIRED(IsKeyPressed);
    LOAD_REQUIRED(IsKeyPressedRepeat);
    LOAD_REQUIRED(IsKeyDown);
    LOAD_REQUIRED(IsKeyReleased);
    LOAD_REQUIRED(IsKeyUp);
    LOAD_REQUIRED(GetKeyPressed);
    LOAD_REQUIRED(GetCharPressed);
    LOAD_REQUIRED(SetExitKey);
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
