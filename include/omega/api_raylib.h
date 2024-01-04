#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_RAYLIB
#include <omega/entry.h>
#if !OMG_RAYLIB_DYNAMIC
#include <raylib.h>
#endif
#define OMG_RAYLIB_PREFIX

#if OMG_RAYLIB_DYNAMIC
typedef enum {
    FLAG_VSYNC_HINT         = 0x00000040,
    FLAG_FULLSCREEN_MODE    = 0x00000002,
    FLAG_WINDOW_RESIZABLE   = 0x00000004,
    FLAG_WINDOW_UNDECORATED = 0x00000008,
    FLAG_WINDOW_HIDDEN      = 0x00000080,
    FLAG_WINDOW_MINIMIZED   = 0x00000200,
    FLAG_WINDOW_MAXIMIZED   = 0x00000400,
    FLAG_WINDOW_UNFOCUSED   = 0x00000800,
    FLAG_WINDOW_TOPMOST     = 0x00001000,
    FLAG_WINDOW_ALWAYS_RUN  = 0x00000100,
    FLAG_WINDOW_TRANSPARENT = 0x00000010,
    FLAG_WINDOW_HIGHDPI     = 0x00002000,
    FLAG_WINDOW_MOUSE_PASSTHROUGH = 0x00004000,
    FLAG_BORDERLESS_WINDOWED_MODE = 0x00008000,
    FLAG_MSAA_4X_HINT       = 0x00000020,
    FLAG_INTERLACED_HINT    = 0x00010000
} ConfigFlags;

typedef struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

typedef struct Vector2 {
    float x;
    float y;
} Vector2;

typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vector3;

typedef struct Vector4 {
    float x;
    float y;
    float z;
    float w;
} Vector4;

typedef Vector4 Quaternion;

typedef struct Matrix {
    float m0, m4, m8, m12;
    float m1, m5, m9, m13;
    float m2, m6, m10, m14;
    float m3, m7, m11, m15;
} Matrix;

typedef struct Rectangle {
    float x;
    float y;
    float width;
    float height;
} Rectangle;

typedef enum {
    MOUSE_BUTTON_LEFT    = 0,
    MOUSE_BUTTON_RIGHT   = 1,
    MOUSE_BUTTON_MIDDLE  = 2,
    MOUSE_BUTTON_SIDE    = 3,
    MOUSE_BUTTON_EXTRA   = 4,
    MOUSE_BUTTON_FORWARD = 5,
    MOUSE_BUTTON_BACK    = 6
} MouseButton;

typedef enum {
    KEY_NULL            = 0,
    KEY_APOSTROPHE      = 39,
    KEY_COMMA           = 44,
    KEY_MINUS           = 45,
    KEY_PERIOD          = 46,
    KEY_SLASH           = 47,
    KEY_ZERO            = 48,
    KEY_ONE             = 49,
    KEY_TWO             = 50,
    KEY_THREE           = 51,
    KEY_FOUR            = 52,
    KEY_FIVE            = 53,
    KEY_SIX             = 54,
    KEY_SEVEN           = 55,
    KEY_EIGHT           = 56,
    KEY_NINE            = 57,
    KEY_SEMICOLON       = 59,
    KEY_EQUAL           = 61,
    KEY_A               = 65,
    KEY_B               = 66,
    KEY_C               = 67,
    KEY_D               = 68,
    KEY_E               = 69,
    KEY_F               = 70,
    KEY_G               = 71,
    KEY_H               = 72,
    KEY_I               = 73,
    KEY_J               = 74,
    KEY_K               = 75,
    KEY_L               = 76,
    KEY_M               = 77,
    KEY_N               = 78,
    KEY_O               = 79,
    KEY_P               = 80,
    KEY_Q               = 81,
    KEY_R               = 82,
    KEY_S               = 83,
    KEY_T               = 84,
    KEY_U               = 85,
    KEY_V               = 86,
    KEY_W               = 87,
    KEY_X               = 88,
    KEY_Y               = 89,
    KEY_Z               = 90,
    KEY_LEFT_BRACKET    = 91,
    KEY_BACKSLASH       = 92,
    KEY_RIGHT_BRACKET   = 93,
    KEY_GRAVE           = 96,
    KEY_SPACE           = 32,
    KEY_ESCAPE          = 256,
    KEY_ENTER           = 257,
    KEY_TAB             = 258,
    KEY_BACKSPACE       = 259,
    KEY_INSERT          = 260,
    KEY_DELETE          = 261,
    KEY_RIGHT           = 262,
    KEY_LEFT            = 263,
    KEY_DOWN            = 264,
    KEY_UP              = 265,
    KEY_PAGE_UP         = 266,
    KEY_PAGE_DOWN       = 267,
    KEY_HOME            = 268,
    KEY_END             = 269,
    KEY_CAPS_LOCK       = 280,
    KEY_SCROLL_LOCK     = 281,
    KEY_NUM_LOCK        = 282,
    KEY_PRINT_SCREEN    = 283,
    KEY_PAUSE           = 284,
    KEY_F1              = 290,
    KEY_F2              = 291,
    KEY_F3              = 292,
    KEY_F4              = 293,
    KEY_F5              = 294,
    KEY_F6              = 295,
    KEY_F7              = 296,
    KEY_F8              = 297,
    KEY_F9              = 298,
    KEY_F10             = 299,
    KEY_F11             = 300,
    KEY_F12             = 301,
    KEY_LEFT_SHIFT      = 340,
    KEY_LEFT_CONTROL    = 341,
    KEY_LEFT_ALT        = 342,
    KEY_LEFT_SUPER      = 343,
    KEY_RIGHT_SHIFT     = 344,
    KEY_RIGHT_CONTROL   = 345,
    KEY_RIGHT_ALT       = 346,
    KEY_RIGHT_SUPER     = 347,
    KEY_KB_MENU         = 348,
    KEY_KP_0            = 320,
    KEY_KP_1            = 321,
    KEY_KP_2            = 322,
    KEY_KP_3            = 323,
    KEY_KP_4            = 324,
    KEY_KP_5            = 325,
    KEY_KP_6            = 326,
    KEY_KP_7            = 327,
    KEY_KP_8            = 328,
    KEY_KP_9            = 329,
    KEY_KP_DECIMAL      = 330,
    KEY_KP_DIVIDE       = 331,
    KEY_KP_MULTIPLY     = 332,
    KEY_KP_SUBTRACT     = 333,
    KEY_KP_ADD          = 334,
    KEY_KP_ENTER        = 335,
    KEY_KP_EQUAL        = 336,
    KEY_BACK            = 4,
    KEY_MENU            = 82,
    KEY_VOLUME_UP       = 24,
    KEY_VOLUME_DOWN     = 25        
} KeyboardKey;

typedef struct Image {
    void* data;
    int width;
    int height;
    int mipmaps;
    int format;
} Image;

typedef struct Texture {
    unsigned int id;
    int width;
    int height;
    int mipmaps;
    int format;
} Texture;

typedef Texture Texture2D;

typedef struct RenderTexture {
    unsigned int id;
    Texture texture;
    Texture depth;
} RenderTexture;

typedef RenderTexture RenderTexture2D;

typedef struct Camera2D {
    Vector2 offset;
    Vector2 target;
    float rotation;
    float zoom;
} Camera2D;

typedef struct Shader {
    unsigned int id;
    int* locs;
} Shader;

typedef enum {
    PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1,
    PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,
    PIXELFORMAT_UNCOMPRESSED_R5G6B5,
    PIXELFORMAT_UNCOMPRESSED_R8G8B8,
    PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,
    PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,
    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    PIXELFORMAT_UNCOMPRESSED_R32,
    PIXELFORMAT_UNCOMPRESSED_R32G32B32,
    PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,
    PIXELFORMAT_UNCOMPRESSED_R16,
    PIXELFORMAT_UNCOMPRESSED_R16G16B16,
    PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,
    PIXELFORMAT_COMPRESSED_DXT1_RGB,
    PIXELFORMAT_COMPRESSED_DXT1_RGBA,
    PIXELFORMAT_COMPRESSED_DXT3_RGBA,
    PIXELFORMAT_COMPRESSED_DXT5_RGBA,
    PIXELFORMAT_COMPRESSED_ETC1_RGB,
    PIXELFORMAT_COMPRESSED_ETC2_RGB,
    PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,
    PIXELFORMAT_COMPRESSED_PVRT_RGB,
    PIXELFORMAT_COMPRESSED_PVRT_RGBA,
    PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,
    PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA
} PixelFormat;

typedef enum {
    BLEND_ALPHA = 0,
    BLEND_ADDITIVE,
    BLEND_MULTIPLIED,
    BLEND_ADD_COLORS,
    BLEND_SUBTRACT_COLORS,
    BLEND_ALPHA_PREMULTIPLY,
    BLEND_CUSTOM,
    BLEND_CUSTOM_SEPARATE
} BlendMode;

typedef enum {
    LOG_ALL = 0,
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
    LOG_NONE
} TraceLogLevel;

typedef void (*TraceLogCallback)(int logLevel, const char *text, va_list args);
#endif

typedef struct {
    void* handle;
    void OMG_RAYLIB_PREFIX (*InitWindow)(int, int, const char*);
    void OMG_RAYLIB_PREFIX (*CloseWindow)(void);
    bool OMG_RAYLIB_PREFIX (*WindowShouldClose)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowReady)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowState)(unsigned int);
    void OMG_RAYLIB_PREFIX (*SetWindowState)(unsigned int);
    void OMG_RAYLIB_PREFIX (*SetWindowTitle)(const char*);
    void OMG_RAYLIB_PREFIX (*SetConfigFlags)(unsigned int);
    void OMG_RAYLIB_PREFIX (*ClearWindowState)(unsigned int);
    void OMG_RAYLIB_PREFIX (*EnableEventWaiting)(void);
    void OMG_RAYLIB_PREFIX (*DisableEventWaiting)(void);
    void OMG_RAYLIB_PREFIX (*TraceLog)(int, const char*, ...);
    void OMG_RAYLIB_PREFIX (*SetTraceLogLevel)(int);
    void* OMG_RAYLIB_PREFIX (*MemAlloc)(unsigned int);
    void* OMG_RAYLIB_PREFIX (*MemRealloc)(void*, unsigned int);
    void OMG_RAYLIB_PREFIX (*MemFree)(void*);
    void OMG_RAYLIB_PREFIX (*WaitTime)(double);
    void OMG_RAYLIB_PREFIX (*SetTargetFPS)(int);
    float OMG_RAYLIB_PREFIX (*GetFrameTime)(void);
    int OMG_RAYLIB_PREFIX (*GetFPS)(void);
    void OMG_RAYLIB_PREFIX (*SetTraceLogCallback)(TraceLogCallback);
    void OMG_RAYLIB_PREFIX (*MaximizeWindow)(void);
    void OMG_RAYLIB_PREFIX (*MinimizeWindow)(void);
    void OMG_RAYLIB_PREFIX (*RestoreWindow)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowResized)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowFullscreen)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowHidden)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowMinimized)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowMaximized)(void);
    bool OMG_RAYLIB_PREFIX (*IsWindowFocused)(void);
    void OMG_RAYLIB_PREFIX (*ToggleFullscreen)(void);
    void OMG_RAYLIB_PREFIX (*ToggleBorderlessWindowed)(void);
    int OMG_RAYLIB_PREFIX (*GetScreenWidth)(void);
    int OMG_RAYLIB_PREFIX (*GetScreenHeight)(void);
    int OMG_RAYLIB_PREFIX (*GetRenderWidth)(void);
    int OMG_RAYLIB_PREFIX (*GetRenderHeight)(void);
    void OMG_RAYLIB_PREFIX (*SetWindowSize)(int, int);
    void OMG_RAYLIB_PREFIX (*SetWindowFocused)(void);
    bool OMG_RAYLIB_PREFIX (*IsMouseButtonPressed)(int);
    bool OMG_RAYLIB_PREFIX (*IsMouseButtonDown)(int);
    bool OMG_RAYLIB_PREFIX (*IsMouseButtonReleased)(int);
    bool OMG_RAYLIB_PREFIX (*IsMouseButtonUp)(int);
    int OMG_RAYLIB_PREFIX (*GetMouseX)(void);
    int OMG_RAYLIB_PREFIX (*GetMouseY)(void);
    Vector2 OMG_RAYLIB_PREFIX (*GetMousePosition)(void);
    Vector2 OMG_RAYLIB_PREFIX (*GetMouseDelta)(void);
    void OMG_RAYLIB_PREFIX (*SetMousePosition)(int, int);
    void OMG_RAYLIB_PREFIX (*SetMouseOffset)(int, int);
    void OMG_RAYLIB_PREFIX (*SetMouseScale)(float, float);
    float OMG_RAYLIB_PREFIX (*GetMouseWheelMove)(void);
    Vector2 OMG_RAYLIB_PREFIX (*GetMouseWheelMoveV)(void);
    void OMG_RAYLIB_PREFIX (*BeginDrawing)(void);
    void OMG_RAYLIB_PREFIX (*EndDrawing)(void);
    void OMG_RAYLIB_PREFIX (*BeginMode2D)(Camera2D);
    void OMG_RAYLIB_PREFIX (*EndMode2D)(void);
    void OMG_RAYLIB_PREFIX (*BeginTextureMode)(RenderTexture2D);
    void OMG_RAYLIB_PREFIX (*EndTextureMode)(void);
    void OMG_RAYLIB_PREFIX (*BeginShaderMode)(Shader);
    void OMG_RAYLIB_PREFIX (*EndShaderMode)(void);
    void OMG_RAYLIB_PREFIX (*BeginBlendMode)(int);
    void OMG_RAYLIB_PREFIX (*EndBlendMode)(void);
    void OMG_RAYLIB_PREFIX (*BeginScissorMode)(int, int, int, int);
    void OMG_RAYLIB_PREFIX (*EndScissorMode)(void);
    Shader OMG_RAYLIB_PREFIX (*LoadShader)(const char*, const char*);
    Shader OMG_RAYLIB_PREFIX (*LoadShaderFromMemory)(const char*, const char*);
    bool OMG_RAYLIB_PREFIX (*IsShaderReady)(Shader);
    int OMG_RAYLIB_PREFIX (*GetShaderLocation)(Shader, const char*);
    int OMG_RAYLIB_PREFIX (*GetShaderLocationAttrib)(Shader, const char*);
    void OMG_RAYLIB_PREFIX (*SetShaderValue)(Shader, int, const void*, int);
    void OMG_RAYLIB_PREFIX (*SetShaderValueV)(Shader, int, const void*, int, int);
    void OMG_RAYLIB_PREFIX (*SetShaderValueMatrix)(Shader, int, Matrix);
    void OMG_RAYLIB_PREFIX (*SetShaderValueTexture)(Shader, int, Texture2D);
    void OMG_RAYLIB_PREFIX (*UnloadShader)(Shader);
    void OMG_RAYLIB_PREFIX (*ClearBackground)(Color);
    bool OMG_RAYLIB_PREFIX (*IsKeyPressed)(int);
    bool OMG_RAYLIB_PREFIX (*IsKeyPressedRepeat)(int);
    bool OMG_RAYLIB_PREFIX (*IsKeyDown)(int);
    bool OMG_RAYLIB_PREFIX (*IsKeyReleased)(int);
    bool OMG_RAYLIB_PREFIX (*IsKeyUp)(int);
    int OMG_RAYLIB_PREFIX (*GetKeyPressed)(void);
    int OMG_RAYLIB_PREFIX (*GetCharPressed)(void);
    void OMG_RAYLIB_PREFIX (*SetExitKey)(int);
} OMG_Raylib;

OMG_API bool omg_raylib_dll_load(OMG_Raylib* this, const OMG_String* dll_path);
OMG_API bool omg_raylib_dll_free(OMG_Raylib* this);

#endif
