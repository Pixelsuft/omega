#pragma once
#include <omega/api_win.h>

#if OMG_SUPPORT_WIN
#include <omega/filesystem.h>

typedef struct {
    OMG_File parent;
    HANDLE handle;
    wchar_t* w_fp;
} OMG_FileWin;
#endif
