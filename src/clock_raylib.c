#include <omega/clock_raylib.h>

#if OMG_SUPPORT_RAYLIB
#define base ((OMG_Clock*)this)

bool omg_clock_raylib_reset(OMG_ClockRaylib* this) {
    this->raylib->GetFrameTime();
    return false;
}

bool omg_clock_raylib_update(OMG_ClockRaylib* this) {
    base->dt = (double)this->raylib->GetFrameTime() * base->speed;
    return false;
}

int omg_clock_raylib_get_fps(OMG_ClockRaylib* this) {
    return this->raylib->GetFPS();
}

bool omg_clock_raylib_set_fps_limit(OMG_ClockRaylib* this, double fps_limit) {
    this->raylib->SetTargetFPS((int)fps_limit);
    return false;
}

bool omg_clock_raylib_init(OMG_ClockRaylib* this, bool high_perf) {
    omg_clock_init((OMG_Clock*)this, high_perf);
    OMG_BEGIN_POINTER_CAST();
    base->reset = omg_clock_raylib_reset;
    base->update = omg_clock_raylib_update;
    base->get_fps = omg_clock_raylib_get_fps;
    base->set_fps_limit = omg_clock_raylib_set_fps_limit;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
