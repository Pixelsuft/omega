#include <omega/scene_objects.h>

#if OMG_SUPPORT_SCENES
bool omg_obj_timer_init(OMG_ObjectTimer* this, OMG_Omega* omg) {
    this->clock = omg->clock;
    this->duration = 0.0;
    this->counter = 0.0;
    return false;
}
#endif
