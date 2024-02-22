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

#if OMG_IS_WIN || OMG_IS_EMSCRIPTEN || OMG_IS_ANDROID
#define OMG_IS_MUSL 0
#else
#define _GNU_SOURCE
#include <features.h>
#ifndef __USE_GNU
#define OMG_IS_MUSL 1
#else
#define OMG_IS_MUSL 0
#endif
#undef _GNU_SOURCE
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

#if !OMG_IS_WIN && !OMG_IS_EMSCRIPTEN && !OMG_IS_ANDROID
#define OMG_IS_UNIX 1
#else
#define OMG_IS_UNIX 0
#endif

#if OMG_IS_CPP
#define OMG_C_EXPORT extern "C"
#else
#define OMG_C_EXPORT
#endif

#if defined(__clang__)
#define OMG_HAVE_FORCE_ALIGN_ARG_POINTER 1
#elif defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2))
#define OMG_HAVE_FORCE_ALIGN_ARG_POINTER 1
#else
#define OMG_HAVE_FORCE_ALIGN_ARG_POINTER 0
#endif
#if defined(__GNUC__) && defined(__i386__) && OMG_HAVE_FORCE_ALIGN_ARG_POINTER
#define OMG_MINGW32_FORCEALIGN __attribute__((force_align_arg_pointer))
#else
#define OMG_MINGW32_FORCEALIGN
#endif

#ifndef OMG_SKIP_STD_DETECTION
#define OMG_HAVE_WCSLEN 0
#define OMG_HAVE__STRREV 0
#define OMG_HAVE_ITOA 0
#define OMG_HAVE__UITOA 0
#define OMG_HAVE_LTOA 0
#define OMG_HAVE__ULTOA 0
#define OMG_HAVE__I64TOA 0
#define OMG_HAVE__UI64TOA 0
#define OMG_HAVE_LRINT OMG_HAS_STD
#define OMG_HAVE_QSORT (OMG_HAS_STD)
#endif

#if OMG_IS_WIN
#if OMG_DLL_BUILD
#define OMG_API OMG_C_EXPORT __declspec(dllexport)
#else
#if OMG_STATIC
#define OMG_API OMG_C_EXPORT
#else
#define OMG_API OMG_C_EXPORT __declspec(dllimport)
#endif
#endif
#else
#if OMG_DLL_BUILD
#define OMG_API __attribute__((visibility("default")))
#else
#define OMG_API OMG_C_EXPORT
#endif
#endif
