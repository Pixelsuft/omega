#include <omega/omega_win.h>
#include <omega/memory_win.h>
#define base ((OMG_Omega*)this)

#if OMG_SUPPORT_WIN
OMG_OmegaWin* omg_win_create(OMG_EntryData* data) {
    OMG_UNUSED(data);
    static OMG_OmegaWin result;
    result.k32 = NULL;
    result.parent.mem = NULL;
    return &result;
}

bool omg_win_destroy(OMG_OmegaWin* this) {
    bool result = false;
    if (base->should_free_mem) {
        result = result || base->mem->destroy(base->mem);
    }
    if (this->should_free_k32) {
        result = result || omg_kernel32_free(this->k32);
    }
    return result;
}

bool omg_win_init(OMG_OmegaWin* this) {
    if (OMG_ISNULL(this->k32)) {
        this->k32 = &this->k32_stk;
        if (omg_kernel32_load(this->k32))
            return false;
        this->should_free_k32 = true;
    }
    else {
        this->should_free_k32 = false;
    }
    OMG_BEGIN_POINTER_CAST();
    omg_init(this);
    if (OMG_ISNULL(base->mem)) {
        base->mem = omg_memory_win_create(this->k32);
        if (OMG_ISNULL(base->mem)) {
            return false; // TODO: free here and probably in other places
        }
        base->should_free_mem = true;
    }
    else {
        base->should_free_mem = false;
    }
    base->destroy = omg_win_destroy;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
