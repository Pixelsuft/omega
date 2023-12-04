#pragma once

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__CYGWIN__) || defined(__MSYS2__)
#define OMG_IS_WIN 1
#else
#define OMG_IS_WIN 0
#endif

#ifdef __EMSCRIPTEN__
#define OMG_IS_EMSCRIPTEN 1
#else
#define OMG_IS_EMSCRIPTEN 0
#endif

#ifdef __android__
#define OMG_IS_ANDROID 1
#else
#define OMG_IS_ANDROID 0
#endif

#ifdef _MSC_VER
#define OMG_IS_VC 1
#else
#define OMG_IS_VC 0
#endif

#ifdef __cplusplus
#define OMG_IS_CPP 1
#else
#define OMG_IS_CPP 0
#endif

#ifdef NOSTDLIB
#define OMG_HAS_STD 0
#else
#define OMG_HAS_STD 1
#endif

#ifdef CMAKE_BUILD
#define OMG_IS_CMAKE 1
#else
#define OMG_IS_CMAKE 0
#endif

#if IS_CPP
#define OMG_C_EXPORT extern "C"
#else
#define OMG_C_EXPORT
#endif

#ifdef OMG_IS_VC
#ifdef OMG_DLL_BUILD
#define OMG_API OMG_C_EXPORT __declspec(dllexport)
#else
#ifdef OMG_STATIC
#define OMG_API OMG_C_EXPORT
#else
#define OMG_API OMG_C_EXPORT __declspec(dllimport)
#endif
#endif
#else
#define OMG_API OMG_C_EXPORT
#endif
