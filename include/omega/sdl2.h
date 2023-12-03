#pragma once
#include <omega/config.h>
#if OMG_SUPPORTS_SDL2
#include <omega/entry.h>
#if !OMG_SDL2_DYNAMIC
// TODO: check if need to just use <SDL.h>
#include <SDL2/SDL.h>
#endif

#if OMG_SDL2_DYNAMIC
#endif

#endif
