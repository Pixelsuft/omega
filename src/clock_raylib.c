#include <omega/clock_raylib.h>

#if OMG_SUPPORT_SDL2
#define base ((OMG_Clock*)this)

bool omg_clock_raylib_reset(OMG_ClockRaylib* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_clock_raylib_update(OMG_ClockRaylib* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_clock_raylib_update_hp(OMG_ClockRaylib* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_clock_raylib_init(OMG_ClockRaylib* this, bool high_perf) {
    omg_clock_init((OMG_Clock*)this, high_perf);
    OMG_BEGIN_POINTER_CAST();
    base->reset = omg_clock_raylib_reset;
    base->update = high_perf ? omg_clock_raylib_update_hp : omg_clock_raylib_update;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
