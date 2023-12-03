#pragma once
#include <omega/platform.h>

#ifndef OMG_DEBUG
#define OMG_DEBUG 1
#endif
#ifndef OMG_ENTRY
#define OMG_ENTRY 1
#endif

#ifndef OMG_SUPPORT_WIN
#define OMG_SUPPORT_WIN 1
#endif
#ifndef OMG_WINAPI_DYNAMIC
#define OMG_WINAPI_DYNAMIC 1
#endif
#ifndef OMG_WIN_BETTER_LIBRARY_LOAD
#define OMG_WIN_BETTER_LIBRARY_LOAD 1
#endif
#ifndef OMG_WINAPI_DYNAMIC_UGLY
#define OMG_WINAPI_DYNAMIC_UGLY 1
#endif
#ifndef OMG_WINAPI_DYNAMIC_COMPAT
#define OMG_WINAPI_DYNAMIC_COMPAT 1
#endif

#ifndef OMG_LOG_MIN_LEVEL
#define OMG_LOG_MIN_LEVEL OMG_LOG_CATEGORY_INFO
#endif
#ifndef OMG_LOG_OMG_MIN_LEVEL
#define OMG_LOG_OMG_MIN_LEVEL OMG_LOG_CATEGORY_INFO
#endif

#ifndef OMG_IMPL_STD_STR_FUNCS
#define OMG_IMPL_BASIC_STD_FUNCS !OMG_HAS_STD
#endif
