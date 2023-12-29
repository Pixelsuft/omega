#pragma once
#include <omega/api_raylib.h>
#include <omega/clock.h>

#if OMG_SUPPORT_SDL2
typedef struct {
    OMG_Clock parent;
    OMG_Raylib* raylib;
} OMG_ClockRaylib;

OMG_API bool omg_clock_raylib_init(OMG_ClockRaylib* this, bool high_perf);
#if OMG_EXPORT_SHIT
OMG_API bool omg_clock_raylib_reset(OMG_ClockRaylib* this);
OMG_API bool omg_clock_raylib_update(OMG_ClockRaylib* this);
OMG_API int omg_clock_raylib_get_fps(OMG_ClockRaylib* this);
OMG_API bool omg_clock_raylib_set_fps_limit(OMG_ClockRaylib* this, double fps_limit);
#endif
#endif
