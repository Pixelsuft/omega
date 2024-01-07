#pragma once
#include <omega/config.h>
#include <omega/ostd.h>

#if OMG_ENTRY
#if OMG_IS_WIN
typedef struct HINSTANCE__ OMG_WIN_ENTRY_HINST;
#else
typedef void OMG_WIN_ENTRY_HINST;
#endif
typedef struct {
    OMG_WIN_ENTRY_HINST* hInst;
    wchar_t* cmdline;
    char* cmdline_s;
    char* argv;
    int cmdshow;
    int argc;
} OMG_EntryData;

// Somewhy SDL2_main doesn't work for me on shitdows with mingw :(
#if OMG_IS_WIN && !(OMG_SUPPORT_SDL2 && !OMG_SDL2_DYNAMIC && OMG_SUPPORT_SDL2_MAIN && OMG_IS_VC)
// #include <omega/api_win.h>

#define _OMG_ENTRY_DATA_TYPE 2

#define OMG_MAIN_MAKE(main_func) int OMG_WIN_STD_PREFIX wWinMain(OMG_WIN_ENTRY_HINST* hInst, OMG_WIN_ENTRY_HINST* hInstPrev, wchar_t* cmdline, int cmdshow) { \
    OMG_UNUSED(hInstPrev); \
    OMG_EntryData entry_data; \
    entry_data.hInst = hInst; \
    entry_data.cmdline = cmdline; \
    entry_data.cmdline_s = NULL; \
    entry_data.argv = NULL; \
    entry_data.argc = -1; \
    entry_data.cmdshow = cmdshow; \
    int _omg_exit_code = main_func(&entry_data); \
    if (!OMG_HAS_STD) \
        ExitProcess((UINT)_omg_exit_code); /* Fuck Microsoft */ \
    return _omg_exit_code; \
} \
\
int OMG_WIN_STD_PREFIX WinMain(OMG_WIN_ENTRY_HINST* hInst, OMG_WIN_ENTRY_HINST* hInstPrev, char* cmdline, int cmdshow) { \
    OMG_UNUSED(hInstPrev); \
    OMG_EntryData entry_data; \
    entry_data.hInst = hInst; \
    entry_data.cmdline = NULL; \
    entry_data.cmdline_s = cmdline; \
    entry_data.argv = NULL; \
    entry_data.argc = -1; \
    entry_data.cmdshow = cmdshow; \
    int _omg_exit_code = main_func(&entry_data); \
    if (!OMG_HAS_STD) \
        ExitProcess((UINT)_omg_exit_code); \
    return _omg_exit_code; \
}
#else
#if OMG_IS_WIN && OMG_SUPPORT_SDL2 && OMG_SUPPORT_SDL2_MAIN && !OMG_SDL2_DYNAMIC && !OMG_IS_VC && OMG_HAS_STD
#define SDL_MAIN_HANDLED
#endif
#define _OMG_ENTRY_DATA_TYPE 1
#if OMG_IS_UNIX && !OMG_HAS_STD
// https://stackoverflow.com/questions/2572988/what-can-you-do-in-c-without-std-includes-are-they-part-of-c-or-just-libra
// https://gist.github.com/tcoppex/443d1dd45f873d96260195d6431b0989
// https://www.reddit.com/r/C_Programming/comments/se3kgi/hello_world_without_libc/?rdt=49181
#define OMG_MAIN_MAKE(main_func) void _start(void) { \
    OMG_EntryData entry_data; \
    entry_data.hInst = NULL; \
    entry_data.cmdline = NULL; \
    entry_data.cmdline_s = NULL; \
    entry_data.argc = 0; \
    entry_data.argv = NULL; \
    main_func(&entry_data); \
    /*asm volatile \
    ( \
        "syscall" \
        :  \
        : "a"(60), "D"(_omg_exit_code) \
        : "rcx", "r11", "memory" \
    ); \*/
}
#else
#define OMG_MAIN_MAKE(main_func) int main(int argc, char* argv[]) { \
    OMG_EntryData entry_data; \
    entry_data.hInst = NULL; \
    entry_data.cmdline = NULL; \
    entry_data.cmdline_s = NULL; \
    entry_data.argc = argc; \
    entry_data.argv = argv; \
    return main_func(&entry_data); \
}
#endif
#endif
#endif
