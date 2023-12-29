#include <omega/clock.h>
#include <omega/omega.h>
#if OMG_HAS_STD && OMG_IS_UNIX
#include <sys/time.h>
#endif
#define omg_base ((OMG_Omega*)this->omg)

bool omg_clock_destroy(OMG_Clock* this) {
    OMG_UNUSED(this);
    return false;
}

int omg_clock_get_fps(OMG_Clock* this) {
    if (this->dt > 0.0) {
        double res = 1.0 / this->dt;
        if ((this->fps_limit > 0.0) && (res > this->fps_limit))
            res = this->fps_limit;
        return (int)(omg_base->std->round(res));
    }
    return (int)(omg_base->std->round(this->fps_limit));
}

bool omg_clock_set_fps_limit(OMG_Clock* this, double fps_limit) {
    this->fps_limit = fps_limit;
    return false;
}

bool omg_clock_reset(OMG_Clock* this) {
#if OMG_HAS_STD && OMG_IS_UNIX
    struct timeval time_value;
    if (gettimeofday(&time_value, NULL))
        return true;
    this->last_tick = (uint64_t)time_value.tv_sec * (uint64_t)1000 + (uint64_t)time_value.tv_usec;
    return false;
#else
    OMG_UNUSED(this);
    return true;
#endif
}

bool omg_clock_update(OMG_Clock* this) {
#if OMG_HAS_STD && OMG_IS_UNIX
    struct timeval time_value;
    if (gettimeofday(&time_value, NULL))
        return true;
    uint64_t now = (uint64_t)time_value.tv_sec * (uint64_t)1000 + (uint64_t)time_value.tv_usec / (uint64_t)1000;
    this->dt = (double)(now - this->last_tick) * this->speed / 1000.0;
    // TODO: fps limit
    this->last_tick = now;
    return false;
#else
    OMG_UNUSED(this);
    return true;
#endif
}

bool omg_clock_update_hp(OMG_Clock* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_clock_init(OMG_Clock* this, bool high_perf) {
    this->high_perf = high_perf;
    this->wait_for_limit = true;
    this->dt = 0.0;
    this->fps_limit = 0.0;
    this->speed = 1.0;
    this->destroy = omg_clock_destroy;
    this->reset = omg_clock_reset;
    this->update = high_perf ? omg_clock_update_hp : omg_clock_update;
    this->get_fps = omg_clock_get_fps;
    this->set_fps_limit = omg_clock_set_fps_limit;
    return false;
}
