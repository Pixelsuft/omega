#pragma once
#include <omega/api_sdl2.h>
#include <omega/clock.h>

#if OMG_SUPPORT_SDL2
typedef struct {
    OMG_Clock parent;
    double freq;
    OMG_Sdl2* sdl2;
} OMG_ClockSdl2;

OMG_API bool omg_clock_sdl2_init(OMG_ClockSdl2* this, bool high_perf);
#if OMG_EXPORT_SHIT
OMG_API bool omg_clock_sdl2_reset(OMG_ClockSdl2* this);
OMG_API bool omg_clock_sdl2_update(OMG_ClockSdl2* this);
OMG_API bool omg_clock_sdl2_update_hp(OMG_ClockSdl2* this);
#endif
#endif
