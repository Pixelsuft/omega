#include <omega/config.h>

#if OMG_SUPPORT_WIN
#include <omega/omega_win.h>
#include <omega/ostd.h>
#include <omega/memory_win.h>
#define base ((OMG_Omega*)this)

#define WIN_STDOUT ((DWORD)-11)
#define WIN_CP_UTF8 65001

OMG_OmegaWin* omg_win_create(OMG_EntryData* data) {
    OMG_UNUSED(data);
    static OMG_OmegaWin result;
    result.k32 = NULL;
    result.parent.mem = NULL;
    result.nt = NULL;
    return &result;
}

void omg_win_log_info_str(OMG_OmegaWin* this, const char* data, size_t size) {
    if (OMG_ISNULL(this->stdout_handle)) {
        this->stdout_handle = this->k32->GetStdHandle(WIN_STDOUT);
        if (OMG_ISNULL(this->stdout_handle)) {
            return;
        }
    }
    if (size == 0)
        size = base->std->strlen(data);
    int count = this->k32->MultiByteToWideChar(WIN_CP_UTF8, 0, data, size, NULL, 0);
    if (count <= 0)
        return;
    wchar_t* out_buf = OMG_MALLOC(base->mem, (size_t)count * 2 + 20);
    if (OMG_ISNULL(out_buf))
        return;
    if (this->k32->MultiByteToWideChar(WIN_CP_UTF8, 0, data, size, out_buf + 8, count) <= 0) {
        OMG_FREE(base->mem, out_buf);
        return;
    }
    base->std->memcpy(out_buf, L"[INFO]: ", 16);
    if (out_buf[count + 7] == L'\n') {
        count--;
    }
    out_buf[count + 8] = '\n';
    out_buf[count + 9] = '\r';
    count += 2;
    DWORD counter = 0;
    this->k32->WriteConsoleW(this->stdout_handle, out_buf, (DWORD)count + 8, &counter, NULL);
    OMG_FREE(base->mem, out_buf);
}

bool omg_win_destroy(OMG_OmegaWin* this) {
    bool result = false;
    if (this->should_free_ntdll) {
        result = omg_winapi_ntdll_free(this->nt) || result;
        result = OMG_FREE(base->mem, this->nt) || result;
        this->nt = NULL;
    }
    if (base->should_free_std) {
        result = OMG_FREE(base->mem, base->std) || result;
        base->std = NULL;
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
            return true;
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
    if (OMG_ISNULL(base->std)) {
        base->std = OMG_MALLOC(base->mem, sizeof(OMG_Std));
        if (OMG_ISNULL(base->std)) {
            omg_winapi_kernel32_free(this->k32);
            this->k32 = NULL;
            return true;
        }
        omg_std_fill_defaults(base->std);
        base->should_free_std = true;
    }
    else {
        base->should_free_std = false;
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
    base->log_info_str = omg_win_log_info_str;
    base->destroy = omg_win_destroy;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
