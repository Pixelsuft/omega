#include <omega/clock_sdl2.h>

#if OMG_SUPPORT_SDL2
#define base ((OMG_Clock*)this)

bool omg_clock_sdl2_reset(OMG_ClockSdl2* this) {
    if (base->high_perf)
        base->last_tick = this->sdl2->SDL_GetPerformanceCounter();
    else
        base->last_tick = this->sdl2->SDL_GetTicks64();
    return false;
}

bool omg_clock_sdl2_update(OMG_ClockSdl2* this) {
    // TODO: fps limit
    uint64_t now = this->sdl2->SDL_GetTicks64();
    base->dt = (double)(now - base->last_tick) * base->speed / 1000.0;
    base->last_tick = now;
    return false;
}

bool omg_clock_sdl2_update_hp(OMG_ClockSdl2* this) {
    uint64_t now = this->sdl2->SDL_GetPerformanceCounter();
    base->dt = (double)(now - base->last_tick) * base->speed / this->freq;
    base->last_tick = now;
    return false;
}

bool omg_clock_sdl2_init(OMG_ClockSdl2* this, bool high_perf) {
    omg_clock_init((OMG_Clock*)this, high_perf);
    if (high_perf)
        this->freq = (double)this->sdl2->SDL_GetPerformanceFrequency();
    OMG_BEGIN_POINTER_CAST();
    base->reset = omg_clock_sdl2_reset;
    base->update = high_perf ? omg_clock_sdl2_update_hp : omg_clock_sdl2_update;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
