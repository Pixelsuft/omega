#include <omega/config.h>

#if OMG_SUPPORT_WIN
#include <omega/omega_win.h>
#include <omega/ostd.h>
#include <omega/memory_win.h>
#define base ((OMG_Omega*)this)

#define WIN_STD_OUTPUT_HANDLE ((DWORD)-11)
#define WIN_STD_ERROR_HANDLE ((DWORD)-12)
#define WIN_CP_UTF8 65001
#define WIN_ATTACH_PARENT_PROCESS ((DWORD)-1)
#define WIN_ERROR_ACCESS_DENIED 0x5
#define WIN_ERROR_INVALID_HANDLE 0x6
#define WIN_ERROR_GEN_FAILURE 0x1F

void omg_win_fill_after_create(OMG_OmegaWin* this) {
    this->k32 = NULL;
    base->mem = NULL;
    this->nt = NULL;
}

OMG_OmegaWin* omg_win_create(OMG_EntryData* data) {
    OMG_UNUSED(data);
    static OMG_OmegaWin result;
    omg_win_fill_after_create(&result);
    return &result;
}

void omg_win_attach_console(OMG_OmegaWin* this) {
    if (this->con_result == 0) {
        BOOL attach_res = this->k32->AttachConsole(WIN_ATTACH_PARENT_PROCESS);
        if (!attach_res) {
            if (this->k32->GetLastError() == WIN_ERROR_ACCESS_DENIED)
                this->con_result = 1;
            else {
                if (!this->k32->AllocConsole()) {
                    this->con_result = -1;
                    return;
                }
                this->con_result = 2;
            }
        }
        else
            this->con_result = 1;
    }
}

void omg_win_log_info_str(OMG_OmegaWin* this, const OMG_String* data) {
    omg_win_attach_console(this);
    if (this->con_result < 0)
        return;
    if (OMG_ISNULL(this->stdout_handle)) {
        this->stdout_handle = this->k32->GetStdHandle(WIN_STD_OUTPUT_HANDLE);
        if (OMG_ISNULL(this->stdout_handle)) {
            return;
        }
    }
    int count = this->k32->MultiByteToWideChar(WIN_CP_UTF8, 0, data->ptr, data->len, NULL, 0);
    if (count <= 0)
        return;
    wchar_t* out_buf = OMG_MALLOC(base->mem, (size_t)count * 2 + 20);
    if (OMG_ISNULL(out_buf))
        return;
    if (this->k32->MultiByteToWideChar(WIN_CP_UTF8, 0, data->ptr, data->len, out_buf + 8, count) <= 0) {
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
    if (this->con_result > 1) {
        result = !this->k32->FreeConsole() || result;
        this->con_result = 0;
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
    this->con_result = 0;
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
        base->std->memory_allocator = base->mem;
        omg_std_set_default_handle(base->std);
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
