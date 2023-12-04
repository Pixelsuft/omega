#pragma once
#include <omega/config.h>
#if OMG_SUPPORTS_SDL2
#include <omega/entry.h>
#if !OMG_SDL2_DYNAMIC
#if OMG_IS_VC || OMG_IS_EMSCRIPTEN || OMG_IS_ANDROID
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#endif

#if OMG_SDL2_DYNAMIC
#endif

#endif
