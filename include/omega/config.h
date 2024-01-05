#pragma once
#include <omega/platform.h>

#ifndef OMG_DEBUG
#define OMG_DEBUG 1
#endif
#ifndef OMG_DEBUG_FILL_MEM
#define OMG_DEBUG_FILL_MEM 1
#endif

#ifndef OMG_ENTRY
#define OMG_ENTRY 1
#endif

#ifndef OMG_SUPPORT_WIN
#define OMG_SUPPORT_WIN OMG_IS_WIN
#endif
#ifndef OMG_WINAPI_DYNAMIC
#define OMG_WINAPI_DYNAMIC 1
#endif
#ifndef OMG_WINAPI_DYNAMIC_UGLY
#define OMG_WINAPI_DYNAMIC_UGLY 1
#endif
#ifndef OMG_WINAPI_DYNAMIC_COMPAT
#define OMG_WINAPI_DYNAMIC_COMPAT 1
#endif
#ifndef OMG_WINAPI_DYNAMIC_ORDINAL
#define OMG_WINAPI_DYNAMIC_ORDINAL 1
#endif
#ifndef OMG_WINAPI_STATIC_COMPAT
#define OMG_WINAPI_STATIC_COMPAT 1
#endif
#ifndef OMG_WIN_BETTER_LIBRARY_LOAD
#define OMG_WIN_BETTER_LIBRARY_LOAD 0
#endif

#ifndef OMG_SUPPORT_SDL2
#define OMG_SUPPORT_SDL2 1
#endif
#ifndef OMG_SDL2_DYNAMIC
#define OMG_SDL2_DYNAMIC 1
#endif
#ifndef OMG_SDL2_COMPAT_DYNAMIC
#define OMG_SDL2_COMPAT_DYNAMIC 0
#endif
#ifndef OMG_SDL2_COMPAT_STATIC
#define OMG_SDL2_COMPAT_STATIC (!OMG_IS_ANDROID && !OMG_IS_EMSCRIPTEN)
#endif
#ifndef OMG_SUPPORT_SDL2_MAIN
#define OMG_SUPPORT_SDL2_MAIN 1
#endif

#ifndef OMG_SUPPORT_RAYLIB
#define OMG_SUPPORT_RAYLIB 1
#endif
#ifndef OMG_RAYLIB_DYNAMIC
#define OMG_RAYLIB_DYNAMIC 1
#endif

#ifndef OMG_USE_LIBMATH
#define OMG_USE_LIBMATH 0
#endif
#ifndef OMG_USE_LIBMATH_EXTRA
#define OMG_USE_LIBMATH_EXTRA OMG_HAS_STD
#endif

#ifndef OMG_EXPORT_SHIT
// Which should be default?
#define OMG_EXPORT_SHIT OMG_DEBUG && 0
#endif

#ifndef OMG_LOG_MIN_LEVEL
#define OMG_LOG_MIN_LEVEL OMG_LOG_CATEGORY_INFO
#endif
#ifndef OMG_LOG_OMG_MIN_LEVEL
#define OMG_LOG_OMG_MIN_LEVEL OMG_LOG_CATEGORY_INFO
#endif
#ifndef OMG_LOG_LIB_MIN_LEVEL
#define OMG_LOG_LIB_MIN_LEVEL OMG_LOG_CATEGORY_INFO
#endif

#ifndef OMG_MAX_WINDOWS
#define OMG_MAX_WINDOWS 5
#endif

#ifndef OMG_WIN_PRECISE_ENCODING
#define OMG_WIN_PRECISE_ENCODING 1
#endif

#ifndef OMG_MAX_COLOR
#define OMG_MAX_COLOR ((omg_color_t)255)
#endif
