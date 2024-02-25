#pragma once
#include <omega/platform.h>

#ifndef OMG_DEBUG
#define OMG_DEBUG 1
#endif
#ifndef OMG_DEBUG_FILL_MEM
#define OMG_DEBUG_FILL_MEM 1
#endif
#ifndef OMG_DEBUG_MEM
#define OMG_DEBUG_MEM OMG_DEBUG && 1
#endif

#ifndef OMG_ENTRY
#define OMG_ENTRY 1
#endif

#ifndef OMG_SUPPORT_LIBC
#define OMG_SUPPORT_LIBC (!OMG_IS_WIN)
#endif
#ifndef OMG_LIBC_DYNAMIC
#define OMG_LIBC_DYNAMIC 1
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

#ifndef OMG_SUPPORT_SDL2_IMAGE
#define OMG_SUPPORT_SDL2_IMAGE OMG_SUPPORT_SDL2
#endif
#ifndef OMG_SDL2_IMAGE_DYNAMIC
#define OMG_SDL2_IMAGE_DYNAMIC OMG_SDL2_DYNAMIC
#endif

#ifndef OMG_SUPPORT_EMSCRIPTEN_AUDIO
#define OMG_SUPPORT_EMSCRIPTEN_AUDIO OMG_IS_EMSCRIPTEN
#endif

#ifndef OMG_SUPPORT_SDL2_MIXER
#define OMG_SUPPORT_SDL2_MIXER OMG_SUPPORT_SDL2 && !OMG_SUPPORT_EMSCRIPTEN_AUDIO
#endif
#ifndef OMG_SDL2_MIXER_DYNAMIC
#define OMG_SDL2_MIXER_DYNAMIC OMG_SDL2_DYNAMIC
#endif
#ifndef OMG_SDL2_MIXER_COMPAT_STATIC
#define OMG_SDL2_MIXER_COMPAT_STATIC (OMG_SDL2_COMPAT_STATIC || OMG_IS_EMSCRIPTEN)
#endif

#ifndef OMG_SUPPORT_SDL2_TTF
#define OMG_SUPPORT_SDL2_TTF OMG_SUPPORT_SDL2
#endif
#ifndef OMG_SDL2_TTF_DYNAMIC
#define OMG_SDL2_TTF_DYNAMIC OMG_SDL2_DYNAMIC
#endif
#ifndef OMG_SDL2_TTF_COMPAT_STATIC
#define OMG_SDL2_TTF_COMPAT_STATIC (OMG_SDL2_COMPAT_STATIC)
#endif

#ifndef OMG_SUPPORT_RAYLIB
#define OMG_SUPPORT_RAYLIB 1
#endif
#ifndef OMG_RAYLIB_DYNAMIC
#define OMG_RAYLIB_DYNAMIC 1
#endif

#ifndef OMG_SUPPORT_FMOD
#define OMG_SUPPORT_FMOD 1
#endif
#ifndef OMG_FMOD_DYNAMIC
#define OMG_FMOD_DYNAMIC 1
#endif

#ifndef OMG_SUPPORT_THREADING
#define OMG_SUPPORT_THREADING 1
#endif

#ifndef OMG_ALLOW_C_RUNTIME
#define OMG_ALLOW_C_RUNTIME 1
#endif

#ifndef OMG_SUPPORT_SCENES
#define OMG_SUPPORT_SCENES 1
#endif

#ifndef OMG_SUPPORT_BEGIN_END_THREAD
#define OMG_SUPPORT_BEGIN_END_THREAD OMG_IS_WIN && OMG_ALLOW_C_RUNTIME && OMG_SUPPORT_THREADING
#endif

#ifndef OMG_USE_LIBMATH
#define OMG_USE_LIBMATH 1
#endif
#ifndef OMG_USE_LIBMATH_EXTRA
#define OMG_USE_LIBMATH_EXTRA OMG_HAS_STD
#endif

#ifndef OMG_IMPL_PEI386_RUNTIME_RELOCATOR
#define OMG_IMPL_PEI386_RUNTIME_RELOCATOR (OMG_IS_WIN && !OMG_IS_VC && !OMG_HAS_STD && OMG_USE_LIBMATH)
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

#ifndef OMG_ALLOW_DUMMY_AUDIO
#define OMG_ALLOW_DUMMY_AUDIO 1
#endif
#ifndef OMG_ALLOW_DUMMY_MUSIC
#define OMG_ALLOW_DUMMY_MUSIC 1
#endif
#ifndef OMG_ALLOW_DUMMY_SOUND
#define OMG_ALLOW_DUMMY_SOUND 1
#endif
#ifndef OMG_ALLOW_DUMMY_SURF
#define OMG_ALLOW_DUMMY_SURF 1
#endif
#ifndef OMG_ALLOW_DUMMY_TEX
#define OMG_ALLOW_DUMMY_TEX 1
#endif
#ifndef OMG_ALLOW_DUMMY_FONT
#define OMG_ALLOW_DUMMY_FONT 1
#endif

#ifndef OMG_ALLOW_SURF_WAS_ALLOCATED
#define OMG_ALLOW_SURF_WAS_ALLOCATED 1
#endif
#ifndef OMG_ALLOW_TEX_WAS_ALLOCATED
#define OMG_ALLOW_TEX_WAS_ALLOCATED 1
#endif

#ifndef OMG_SUPPORT_OMG_IMAGE
#define OMG_SUPPORT_OMG_IMAGE 1
#endif
#ifndef OMG_SUPPORT_SPNG
#define OMG_SUPPORT_SPNG OMG_SUPPORT_OMG_IMAGE
#endif
#ifndef OMG_SPNG_DYNAMIC
#define OMG_SPNG_DYNAMIC 1
#endif

#ifndef OMG_MAX_PLAYING_MUSIC
#define OMG_MAX_PLAYING_MUSIC 8
#endif
#ifndef OMG_MAX_PLAYING_SOUND
#define OMG_MAX_PLAYING_SOUND 8
#endif

#ifndef OMG_USE_RADIANS
#define OMG_USE_RADIANS 1
#endif

#ifndef OMG_MAX_COLOR
#define OMG_MAX_COLOR ((omg_color_t)255)
#endif
