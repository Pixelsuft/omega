#include <omega/config.h>

#if OMG_SUPPORT_WIN
#include <omega/omega_win.h>
#include <omega/memory_win.h>
#define base ((OMG_Omega*)this)

OMG_OmegaWin* omg_win_create(OMG_EntryData* data) {
    OMG_UNUSED(data);
    static OMG_OmegaWin result;
    result.k32 = NULL;
    result.parent.mem = NULL;
    result.nt = NULL;
    return &result;
}

bool omg_win_destroy(OMG_OmegaWin* this) {
    bool result = false;
    if (this->should_free_ntdll) {
        result = omg_winapi_ntdll_free(this->nt) || result;
        result = OMG_FREE(base->mem, this->nt) || result;
        this->nt = NULL;
    }
    if (base->should_free_mem) {
        result = base->mem->destroy(base->mem) || result;
        base->mem = NULL;
    }
    if (this->should_free_k32) {
        result = omg_winapi_kernel32_free(this->k32) || result;
        this->k32 = NULL;
    }
    return result;
}

bool omg_win_init(OMG_OmegaWin* this) {
    if (OMG_ISNULL(this->k32)) {
        this->k32 = &this->k32_stk;
        if (omg_winapi_kernel32_load(this->k32))
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
            omg_winapi_kernel32_free(this->k32);
            this->k32 = NULL;
            return true;
        }
        base->should_free_mem = true;
    }
    else {
        base->should_free_mem = false;
    }
    if (OMG_ISNULL(this->nt)) {
        this->nt = OMG_MALLOC(base->mem, sizeof(OMG_Ntdll));
        if (OMG_ISNULL(this->nt)) {
            base->mem->destroy(base->mem);
            base->mem = NULL;
            omg_winapi_kernel32_free(this->k32);
            this->k32 = NULL;
            return true;
        }
        if (omg_winapi_ntdll_load(this->nt)) {
            OMG_FREE(base->mem, this->nt);
            base->mem->destroy(base->mem);
            base->mem = NULL;
            omg_winapi_kernel32_free(this->k32);
            this->k32 = NULL;
            return true;
        }
        this->should_free_ntdll = true;
    }
    else {
        this->should_free_ntdll = false;
    }
    base->destroy = omg_win_destroy;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
