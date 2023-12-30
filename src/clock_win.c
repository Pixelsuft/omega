#include <omega/clock_win.h>

#if OMG_SUPPORT_WIN
#define base ((OMG_Clock*)this)

bool omg_clock_win_reset(OMG_ClockWin* this) {
    if (base->high_perf) {
        this->k32->QueryPerformanceCounter(&this->temp_int);
        base->last_tick = (uint64_t)this->temp_int.QuadPart;
    }
    else
        base->last_tick = (uint64_t)this->k32->GetTickCount64();
    return false;
}

bool omg_clock_win_update(OMG_ClockWin* this) {
    uint64_t now = (uint64_t)this->k32->GetTickCount64();
    base->dt = (double)(now - base->last_tick) * base->speed / 1000.0;
    if ((base->fps_limit >= 0.0) && (base->dt < (base->dt_limit * base->speed))) {
        if (!base->wait_for_limit)
            return true;
        while (base->dt < (base->dt_limit * base->speed)) {
            now = (uint64_t)this->k32->GetTickCount64();
            base->dt = (double)(now - base->last_tick) * base->speed / 1000.0;
        }
    }
    base->last_tick = now;
    return false;
}

bool omg_clock_win_update_hp(OMG_ClockWin* this) {
    this->k32->QueryPerformanceCounter(&this->temp_int);
    uint64_t now = (uint64_t)this->temp_int.QuadPart;
    base->dt = (double)(now - base->last_tick) * base->speed / this->freq;
    if ((base->fps_limit >= 0.0) && (base->dt < (base->dt_limit * base->speed))) {
        if (!base->wait_for_limit)
            return true;
        while (false && base->dt < (base->dt_limit * base->speed)) { // TODO: It's broken
            now = (uint64_t)this->temp_int.QuadPart;
            base->dt = (double)(now - base->last_tick) * base->speed / this->freq;
        }
    }
    base->last_tick = now;
    return false;
}

bool omg_clock_win_init(OMG_ClockWin* this, bool high_perf) {
    omg_clock_init((OMG_Clock*)this, high_perf);
    this->temp_int.QuadPart = 1000;
    if (high_perf) {
        this->k32->QueryPerformanceFrequency(&this->temp_int);
        this->freq = (double)this->temp_int.QuadPart;
    }
    OMG_BEGIN_POINTER_CAST();
    base->reset = omg_clock_win_reset;
    base->update = high_perf ? omg_clock_win_update_hp : omg_clock_win_update;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
