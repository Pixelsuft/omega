#pragma once
#include <omega/ostd.h>

typedef struct OMG_Clock {
    uint64_t last_tick;
    uint64_t last_tick2;
    void* omg;
    /* Init clock. high_perf means precision higer than 1ms */
    bool (*init)(struct OMG_Clock* this, bool high_perf);
    /* Destroy clock */
    bool (*destroy)(struct OMG_Clock* this);
    /* Reset clock timer */
    bool (*reset)(struct OMG_Clock* this);
    /* Update clock and update dt (delta time) */
    bool (*update)(struct OMG_Clock* this);
    bool (*set_fps_limit)(struct OMG_Clock* this, double fps_limit);
    int (*get_fps)(struct OMG_Clock* this);
    double dt;
    double fps_limit;
    double dt_limit;
    double speed;
    bool wait_for_limit;
    bool high_perf;
    bool inited;
    bool was_allocated;
} OMG_Clock;

OMG_API bool omg_clock_init(OMG_Clock* this, bool high_perf);
#if OMG_EXPORT_SHIT
OMG_API int omg_clock_get_fps(OMG_Clock* this);
OMG_API bool omg_clock_set_fps_limit(OMG_Clock* this, double fps_limit);
OMG_API bool omg_clock_reset(OMG_Clock* this);
OMG_API bool omg_clock_update(OMG_Clock* this);
OMG_API bool omg_clock_update_hp(OMG_Clock* this);
OMG_API bool omg_clock_destroy(OMG_Clock* this);
#endif
