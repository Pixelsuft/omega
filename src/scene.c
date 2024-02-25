#include <omega/scene.h>

#if OMG_SUPPORT_SCENES
#include <omega/omega.h>
#include <omega/renderer.h>

#define ren ((OMG_Renderer*)this->omg_ren)
#define omg_base ((OMG_Omega*)this->omg_omg)

bool omg_scenemgr_init(OMG_SceneMgr* this, void* omg_ren) {
    this->omg_ren = omg_ren;
    this->omg_omg = ren->omg;
    _OMG_LOG_INFO(omg_base, "Omega scenes successfully inited");
    return false;
}

bool omg_scenemgr_destroy(OMG_SceneMgr* this) {
    OMG_UNUSED(this);
    return false;
}
#endif
