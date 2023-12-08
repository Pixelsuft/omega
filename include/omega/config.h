#pragma once
#include <omega/platform.h>

#ifndef OMG_DEBUG
#define OMG_DEBUG 1
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
#ifndef OMG_WIN_BETTER_LIBRARY_LOAD
#define OMG_WIN_BETTER_LIBRARY_LOAD 0
#endif

#ifndef OMG_SUPPORT_SDL2
#define OMG_SUPPORT_SDL2 1
#endif
#ifndef OMG_SDL2_DYNAMIC
#define OMG_SDL2_DYNAMIC 1
#endif
#ifndef OMG_SUPPORT_SDL2_MAIN
#define OMG_SUPPORT_SDL2_MAIN 1
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

#ifndef OMG_WIN_PRECISE_ENCODING
#define OMG_WIN_PRECISE_ENCODING 1
#endif
