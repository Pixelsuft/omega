#include <omega/scene.h>

#if OMG_SUPPORT_SCENES
#include <omega/omega.h>
#include <omega/window.h>
#include <omega/renderer.h>

#define ren ((OMG_Renderer*)this->omg_ren)
#define omg_base ((OMG_Omega*)this->omg_omg)

bool omg_scenemgr_init(OMG_SceneMgr* this, void* omg_ren) {
    this->omg_ren = omg_ren;
    this->omg_omg = ren->omg;
    // omg_base->std->memset((void*)((size_t)this->on_update), 0, (size_t)this->on_touch_move - (size_t)this->on_update);
    omg_base->std->memcpy(
        (void*)((size_t)this->on_update),
        (void*)((size_t)omg_base->on_update),
        (size_t)this->on_touch_move - (size_t)this->on_update
    );
    _OMG_LOG_INFO(omg_base, "Omega scenes successfully inited");
    return false;
}

bool omg_scenemgr_destroy(OMG_SceneMgr* this) {
    omg_base->std->memcpy(
        (void*)((size_t)omg_base->on_update),
        (void*)((size_t)this->on_update),
        (size_t)this->on_touch_move - (size_t)this->on_update
    );
    return false;
}
#endif
