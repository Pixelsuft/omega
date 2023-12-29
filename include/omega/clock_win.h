#pragma once
#include <omega/api_win.h>
#include <omega/clock.h>

#if OMG_SUPPORT_SDL2
typedef struct {
    OMG_Clock base;
    OMG_WIN_LARGE_INTEGER temp_int;
    double freq;
    OMG_Kernel32* k32;
} OMG_ClockWin;

OMG_API bool omg_clock_win_init(OMG_ClockWin* this, bool high_perf);
#if OMG_EXPORT_SHIT
OMG_API bool omg_clock_win_reset(OMG_ClockWin* this);
OMG_API bool omg_clock_win_update(OMG_ClockWin* this);
OMG_API bool omg_clock_win_update_hp(OMG_ClockWin* this);
#endif
#endif
