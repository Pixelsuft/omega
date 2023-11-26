#pragma once
#include <omega/config.h>
#include <omega/config.h>
#include <omega/ostd.h>

#if OMG_ENTRY
#if OMG_IS_WIN
#include <omega/winapi.h>

typedef struct {
    HINSTANCE hInst;
    PWSTR cmdline;
    PSTR cmdline_s;
    int cmdshow;
} OMG_EntryData;

#define OMG_MAKE_MAIN(main_func) int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PWSTR cmdline, int cmdshow) { \
    OMG_UNUSED(hInstPrev); \
    OMG_EntryData entry_data; \
    entry_data.hInst = hInst; \
    entry_data.cmdline = cmdline; \
    entry_data.cmdline_s = NULL; \
    entry_data.cmdshow = cmdshow; \
    return main_func(&entry_data); \
} \
\
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) { \
    OMG_UNUSED(hInstPrev); \
    OMG_EntryData entry_data; \
    entry_data.hInst = hInst; \
    entry_data.cmdline = NULL; \
    entry_data.cmdline_s = cmdline; \
    entry_data.cmdshow = cmdshow; \
    return main_func(&entry_data); \
}
#else
typedef struct {
    int argc;
    char** argv;
} OMG_EntryData;

#define OMG_MAKE_MAIN(main_func) int main(int argc, char* argv[]) { \
    OMG_EntryData entry_data; \
    entry_data.argc = argc; \
    entry_data.argv = argv; \
    return main_func(&entry_data); \
}
#endif
#endif
