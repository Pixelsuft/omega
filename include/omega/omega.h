#pragma once
#include <omega/config.h>
#include <omega/ostd.h>
#include <omega/entry.h>
#include <omega/memory.h>
#include <omega/clock.h>
#include <omega/winmgr.h>
#include <omega/window.h>
#include <omega/events.h>
#include <omega/input.h>
#include <omega/filesystem.h>
#include <omega/audio.h>
#include <omega/threading.h>

#define OMG_OMEGA_TYPE_NONE 0
#define OMG_OMEGA_TYPE_WIN 1
#define OMG_OMEGA_TYPE_SDL2 2
#define OMG_OMEGA_TYPE_RAYLIB 3

#define OMG_THEME_NONE 0
#define OMG_THEME_LIGHT 1
#define OMG_THEME_DARK 2
#define OMG_THEME_AUTO 3

#define OMG_LOG_CATEGORY_INFO 1
#define OMG_LOG_CATEGORY_WARN 2
#define OMG_LOG_CATEGORY_ERROR 3
#define OMG_LOG_CATEGORY_FATAL 4
#define OMG_LOG_CATEGORY_DISABLED 5

#define OMG_TEXT_INPUT_UNCHANGED 0
#define OMG_TEXT_INPUT_DISABLED 1
#define OMG_TEXT_INPUT_ENABLED 2

#define OMG_TOUCH_MOUSEID ((uint32_t)-1)
#define OMG_MOUSE_TOUCHID ((int64)-1)
#define OMG_MBUTTON(X)       (1 << ((X) - 1))
#define OMG_MBUTTON_LEFT     1
#define OMG_MBUTTON_MIDDLE   2
#define OMG_MBUTTON_RIGHT    3
#define OMG_MBUTTON_X1       4
#define OMG_MBUTTON_X2       5
#define OMG_MBUTTON_X3       6
#define OMG_MBUTTON_X4       7
#define OMG_MBUTTON_LMASK    OMG_MBUTTON(OMG_MBUTTON_LEFT)
#define OMG_MBUTTON_MMASK    OMG_MBUTTON(OMG_MBUTTON_MIDDLE)
#define OMG_MBUTTON_RMASK    OMG_MBUTTON(OMG_MBUTTON_RIGHT)
#define OMG_MBUTTON_X1MASK   OMG_MBUTTON(OMG_MBUTTON_X1)
#define OMG_MBUTTON_X2MASK   OMG_MBUTTON(OMG_MBUTTON_X2)
#define OMG_MBUTTON_X3MASK   OMG_MBUTTON(OMG_MBUTTON_X3)
#define OMG_MBUTTON_X4MASK   OMG_MBUTTON(OMG_MBUTTON_X4)

#define OMG_MESSAGEBOX_ERROR 0x00000010
#define OMG_MESSAGEBOX_WARN 0x00000020
#define OMG_MESSAGEBOX_INFO 0x00000040

#define _OMG_CHECK_DARK_MODE_INTERNAL() do { \
    if (OMG_ISNOTNULL(d_uxtheme->AllowDarkModeForApp)) \
        d_uxtheme->AllowDarkModeForApp((base->app_theme == OMG_THEME_DARK) || (base->app_theme == OMG_THEME_AUTO)); \
    if (OMG_ISNOTNULL(d_uxtheme->SetPreferredAppMode)) { \
        if (base->app_theme == OMG_THEME_AUTO) \
            d_uxtheme->SetPreferredAppMode(OMG_WIN_APPMODE_ALLOW_DARK); \
        else if (base->app_theme == OMG_THEME_NONE) \
            d_uxtheme->SetPreferredAppMode(OMG_WIN_APPMODE_DEFAULT); \
        else if (base->app_theme == OMG_THEME_LIGHT) \
            d_uxtheme->SetPreferredAppMode(OMG_WIN_APPMODE_FORCE_LIGHT); \
        else if (base->app_theme == OMG_THEME_DARK) \
            d_uxtheme->SetPreferredAppMode(OMG_WIN_APPMODE_FORCE_DARK); \
    } \
    if (OMG_ISNOTNULL(d_uxtheme->ShouldSystemUseDarkMode)) \
        base->theme = d_uxtheme->ShouldSystemUseDarkMode() ? OMG_THEME_DARK : OMG_THEME_LIGHT; \
} while (0)

#define _OMG_MSGBOX_DEF_FILL_TITLE(title, title_ptr) do { \
    if (OMG_ISNULL(title)) { \
        if (flags & OMG_MESSAGEBOX_INFO) \
            title_ptr = "Information!"; \
        else if (flags & OMG_MESSAGEBOX_WARN) \
            title_ptr = "Warning!"; \
        else if (flags & OMG_MESSAGEBOX_ERROR) \
            title_ptr = "Error!"; \
        else \
            title_ptr = "Alert!"; \
    } \
    else \
        title_ptr = title->ptr; \
} while (0)

typedef struct OMG_Omega {
    OMG_Memory* mem;
    OMG_Std* std;
    OMG_EntryData* entry_data;
    OMG_Winmgr* winmgr;
    OMG_Clock* clock;
    OMG_Audio* audio;
    OMG_Window** omg_window_cache;
    OMG_String* sdl2_dll_path;
    OMG_String* sdl2_image_dll_path;
    OMG_String* sdl2_mixer_dll_path;
    OMG_String* sdl2_ttf_dll_path;
    OMG_String* fmod_dll_path;
    OMG_String* spng_dll_path;
    OMG_String* libc_dll_path;
    bool* keyboard_state;
    const OMG_Keycode* keymap;
    void* extra1;
    void* extra2;
    void* extra3;
    void* extra4;
    void* extra5;
    void* event_arg;
    size_t sz_file;
    /* Main init */
    bool (*omg_init)(struct OMG_Omega* this);
    /* Second init */
    bool (*app_init)(struct OMG_Omega* this);
    /* Second init cleanup */
    bool (*app_quit)(struct OMG_Omega* this);
    /* Main init cleanup */
    bool (*destroy)(struct OMG_Omega* this);
    /* Free window manager */
    bool (*winmgr_free)(struct OMG_Omega* this);
    /* Allocate window manager */
    bool (*winmgr_alloc)(struct OMG_Omega* this);
    /* Sleep seconds */
    void (*delay)(struct OMG_Omega* this, double seconds);
    /* Set log level */
    void (*log_set_level)(struct OMG_Omega* this, const int log_level, const int omg_log_level, const int lib_log_level);
    /* Log string */
    bool (*log_str_type)(struct OMG_Omega* this, const OMG_String* data, int type);
    /* Run simple main loop */
    void (*auto_loop_run)(struct OMG_Omega* this);
    /* Stop simple main loop */
    void (*auto_loop_stop)(struct OMG_Omega* this);
    /* Set/query text input enabled. OMG_TEXT_INPUT_ENABLED - enable, OMG_TEXT_INPUT_DISABLED - disable, other - query */
    int (*set_text_input_state)(struct OMG_Omega* this, int state);
    /* Allocate audio manager */
    bool (*audio_alloc)(struct OMG_Omega* this);
    /* Free audio manager */
    bool (*audio_free)(struct OMG_Omega* this);
    /* Is file/dir. type 0 - file, 1 - dir, 2 - any of */
    bool (*fs_is_file_or_dir)(struct OMG_Omega* this, const OMG_String* path, int type);
    /* Remove file/dir. type 0 - file, 1 - dir, 2 - recursive dir (unsupported) */
    bool (*fs_remove_file_or_dir)(struct OMG_Omega* this, const OMG_String* path, int type);
    /* Rename/move dir/file */
    bool (*fs_move)(struct OMG_Omega* this, const OMG_String* old_path, const OMG_String* new_path);
    bool (*fs_create_dir)(struct OMG_Omega* this, const OMG_String* path);
    /* Get environment variable. The returned string must be freed */
    OMG_String (*env_get)(struct OMG_Omega* this, const OMG_String* key_name);
    /* Set environment variable */
    bool (*env_set)(struct OMG_Omega* this, const OMG_String* key_name, const OMG_String* key_value, bool overwrite);
    /* Create file object by file path */
    OMG_File* (*file_from_fp)(struct OMG_Omega* this, OMG_File* file, const OMG_String* path, int mode);
    /* Create file object by data from memory */
    OMG_File* (*file_from_mem)(struct OMG_Omega* this, OMG_File* file, const void* mem, size_t size, bool read_only);
    /* Show simple message box */
    bool (*message_box)(struct OMG_Omega* this, const OMG_String* text, const OMG_String* title, int flags);
    /* Create thread. Use OMG_THREAD_CREATE macro please */
    OMG_Thread* (*thread_create)(struct OMG_Omega* this, OMG_ThreadFunction func, const OMG_String* name, void* data, size_t stack_size, void* reserved1, void* reserved2);
    /* Get thread id. NULL thread means current */
    size_t (*thread_get_id)(struct OMG_Omega* this, OMG_Thread* thread);
    bool (*thread_set_priority)(struct OMG_Omega* this, OMG_Thread* thread, int priority);
    /* Wait until thread finishes. Status pointer may be NULL */
    bool (*thread_wait)(struct OMG_Omega* this, OMG_Thread* thread, int* status);
    /* Cleanup thread before or after it's finished, if you don't need to use omg thread functions */
    bool (*thread_detach)(struct OMG_Omega* this, OMG_Thread* thread);
    /* Set default event handlers */
    void (*reset_event_handlers)(struct OMG_Omega* this);
    /* Get current working directory or app executable dir */
    OMG_String (*get_cwd)(struct OMG_Omega* this, bool base_dir);
    /* Quit event */
    void (*on_quit)(OMG_EventQuit* event);
    /* Update event */
    void (*on_update)(OMG_EventUpdate* event);
    /* Loop stop event */
    void (*on_loop_stop)(OMG_EventLoopStop* event);
    /* Paint event */
    void (*on_paint)(OMG_EventPaint* event);
    /* Expose event (you can do redraw here) */
    void (*on_expose)(OMG_EventExpose* event);
    /* Size change event (including resizing from code) */
    void (*on_size_change)(OMG_EventResize* event);
    /* Mouse move event */
    void (*on_mouse_move)(OMG_EventMouseMove* event);
    /* Mouse button down event */
    void (*on_mouse_down)(OMG_EventMouseButton* event);
    /* Mouse button up event */
    void (*on_mouse_up)(OMG_EventMouseButton* event);
    /* Mouse wheel event */
    void (*on_mouse_wheel)(OMG_EventMouseWheel* event);
    /* Mouse enter event */
    void (*on_mouse_enter)(OMG_EventMouseFocus* event);
    /* Mouse leave event */
    void (*on_mouse_leave)(OMG_EventMouseFocus* event);
    /* Key down event */
    void (*on_key_down)(OMG_EventKeyboard* event);
    /* Key up event */
    void (*on_key_up)(OMG_EventKeyboard* event);
    /* Text input event */
    void (*on_text_input)(OMG_EventTextInput* event);
    /* Touch down event */
    void (*on_touch_down)(OMG_EventTouch* event);
    /* Touch up event */
    void (*on_touch_up)(OMG_EventTouch* event);
    /* Touch move event */
    void (*on_touch_move)(OMG_EventTouch* event);
    /* Close event */
    void (*on_close)(OMG_EventClose* event);
    /* Resize event */
    void (*on_resize)(OMG_EventResize* event);
    /* State change event */
    void (*on_state_change)(OMG_EventStateChange* event);
    /* State changing event */
    void (*on_state_changing)(OMG_EventStateChanging* event);
    /* Focus change event */
    void (*on_focus_change)(OMG_EventFocusChange* event);
#if OMG_IS_WIN
    uint64_t k32_stk[64];
    void* k32;
    void* nt;
    void* dwm;
    void* u32;
    void* g32;
    void* uxtheme;
    void* msvcrt;
#endif
#if OMG_SUPPORT_LIBC
    void* libc;
#endif
    OMG_FPoint scale;
    uint32_t mouse_state;
    int log_level_lib;
    int log_level_omg;
    int log_level;
    int type;
    int audio_type;
    int theme;
    int app_theme;
    int win_major_ver;
    int win_minor_ver;
    int win_build_number;
    bool emulate_mouse;
    bool enable_paint;
    bool looping;
    bool supports_screen_keyboard;
    bool support_highdpi;
    bool should_free_mem;
    bool should_free_std;
#if OMG_IS_WIN
    bool should_free_k32;
    bool should_free_ntdll;
    bool should_free_dwm;
    bool should_free_u32;
    bool should_free_g32;
    bool should_free_uxtheme;
    bool should_free_msvcrt;
#endif
#if OMG_SUPPORT_LIBC
    bool should_free_libc;
#endif
    bool inited;
} OMG_Omega;

OMG_API OMG_Omega* omg_alloc_omega_internal(OMG_EntryData* data, size_t size);
OMG_API void omg_free_omega_internal(OMG_Omega* this);
OMG_API OMG_Omega* omg_create(OMG_EntryData* data);
OMG_API OMG_Omega* omg_create_by_type(OMG_EntryData* data, int omg_type);
OMG_API void omg_fill_on_create(OMG_Omega* this, OMG_EntryData* data);
OMG_API OMG_Omega* omg_get_default_omega(void);
OMG_API bool omg_destroy(OMG_Omega* this);
OMG_API bool omg_omg_init(OMG_Omega* this);
OMG_API void omg_reset_event_handlers(OMG_Omega* this);
OMG_API bool omg_alloc_winmgr(OMG_Omega* this);
OMG_API bool omg_audio_alloc(OMG_Omega* this);
OMG_API bool omg_app_init(OMG_Omega* this);
OMG_API bool omg_app_quit(OMG_Omega* this);
OMG_API bool omg_dummy_audio_alloc(OMG_Omega* this);
OMG_API OMG_File* omg_file_from_fp(OMG_Omega* this, OMG_File* file, const OMG_String* path, int mode);
OMG_API bool omg_file_destroy(OMG_File* file);
OMG_API bool omg_win_destroy_clean1(OMG_Omega* this);
OMG_API bool omg_win_destroy_clean2(OMG_Omega* this);
OMG_API bool omg_win_loads_libs1(OMG_Omega* this);
OMG_API bool omg_win_loads_libs2(OMG_Omega* this);
OMG_API bool omg_win_loads_libs3(OMG_Omega* this);
OMG_API bool omg_libc_init(OMG_Omega* this);
OMG_API bool omg_libc_destroy(OMG_Omega* this);
OMG_API void omg_event_on_quit(OMG_EventQuit* event);
OMG_API void omg_event_on_update(OMG_EventUpdate* event);
OMG_API void omg_event_on_loop_stop(OMG_EventLoopStop* event);
OMG_API void omg_event_on_paint(OMG_EventPaint* event);
OMG_API void omg_event_on_expose(OMG_EventExpose* event);
OMG_API void omg_event_on_close(OMG_EventClose* event);
OMG_API void omg_event_on_resize(OMG_EventResize* event);
OMG_API void omg_event_on_state_change(OMG_EventStateChange* event);
OMG_API void omg_event_on_state_changing(OMG_EventStateChanging* event);
OMG_API void omg_event_on_focus_change(OMG_EventFocusChange* event);
OMG_API void omg_event_on_mouse_move(OMG_EventMouseMove* event);
OMG_API void omg_event_on_mouse_button(OMG_EventMouseButton* event);
OMG_API void omg_event_on_mouse_wheel(OMG_EventMouseWheel* event);
OMG_API void omg_event_on_mouse_focus(OMG_EventMouseFocus* event);
OMG_API void omg_event_on_keyboard(OMG_EventKeyboard* event);
OMG_API void omg_event_on_text_input(OMG_EventTextInput* event);
OMG_API void omg_event_on_touch(OMG_EventTouch* event);
#if OMG_EXPORT_SHIT
#include <omega/filesystem.h>

OMG_API OMG_String omg_get_cwd(OMG_Omega* this, bool base_dir);
OMG_API OMG_Thread* omg_thread_create(OMG_Omega* this, OMG_ThreadFunction func, const OMG_String* name, void* data, size_t stack_size, void* reserved1, void* reserved2);
OMG_API size_t omg_thread_get_id(OMG_Omega* this, OMG_Thread* thread);
OMG_API bool omg_thread_set_priority(OMG_Omega* this, OMG_Thread* thread, int priority);
OMG_API bool omg_thread_wait(OMG_Omega* this, OMG_Thread* thread, int* status);
OMG_API bool omg_thread_detach(OMG_Omega* this, OMG_Thread* thread);
OMG_API bool omg_message_box(OMG_Omega* this, const OMG_String* text, const OMG_String* title, int flags);
OMG_API OMG_String omg_env_get(OMG_Omega* this, const OMG_String* key_name);
OMG_API bool omg_env_set(OMG_Omega* this, const OMG_String* key_name, const OMG_String* key_value, bool overwrite);
OMG_API bool omg_fs_move(OMG_Omega* this, const OMG_String* old_path, const OMG_String* new_path);
OMG_API bool omg_fs_is_file_or_dir(OMG_Omega* this, const OMG_String* path, int type);
OMG_API bool omg_fs_remove_file_or_dir(OMG_Omega* this, const OMG_String* path, int type);
OMG_API bool omg_fs_create_dir(OMG_Omega* this, const OMG_String* path);
OMG_API int64_t omg_file_get_size(OMG_File* file);
OMG_API int64_t omg_file_seek(OMG_File* file, int64_t offset, int whence);
OMG_API int64_t omg_file_tell(OMG_File* file);
OMG_API size_t omg_file_read(OMG_File* file, void* buf, size_t size, size_t maxnum);
OMG_API size_t omg_file_write(OMG_File* file, const void* buf, size_t size, size_t num);
OMG_API OMG_FileMem* omg_file_from_mem(OMG_Omega* this, OMG_FileMem* file, const void* mem, size_t size, bool read_only);
OMG_API int64_t omg_file_mem_get_size(OMG_FileMem* file);
OMG_API int64_t omg_file_mem_seek(OMG_FileMem* file, int64_t offset, int whence);
OMG_API int64_t omg_file_mem_tell(OMG_FileMem* file);
OMG_API size_t omg_file_mem_read(OMG_FileMem* file, void* buf, size_t size, size_t maxnum);
OMG_API size_t omg_file_mem_write(OMG_FileMem* file, const void* buf, size_t size, size_t num);
#if OMG_HAS_STD
OMG_API OMG_FileStd* omg_file_std_from_path(OMG_Omega* this, OMG_FileStd* file, const OMG_String* path, int mode);
OMG_API bool omg_file_std_destroy(OMG_FileStd* file);
OMG_API int64_t omg_file_std_get_size(OMG_FileStd* file);
OMG_API int64_t omg_file_std_seek(OMG_FileStd* file, int64_t offset, int whence);
OMG_API int64_t omg_file_std_tell(OMG_FileStd* file);
OMG_API size_t omg_file_std_read(OMG_FileStd* file, void* buf, size_t size, size_t maxnum);
OMG_API size_t omg_file_std_write(OMG_FileStd* file, const void* buf, size_t size, size_t num);
#endif
OMG_API int omg_set_text_input_state(OMG_Omega* this, int state);
OMG_API bool omg_free_winmgr(OMG_Omega* this);
OMG_API bool omg_audio_free(OMG_Omega* this);
OMG_API void omg_delay(OMG_Omega* this, double seconds);
OMG_API void omg_log_set_level(OMG_Omega* this, const int log_level, const int omg_log_level, const int lib_log_level);
OMG_API bool omg_log_str_type(OMG_Omega* this, const OMG_String* data, int type);
OMG_API void omg_auto_loop_run(OMG_Omega* this);
OMG_API void omg_auto_loop_stop(OMG_Omega* this);
#if OMG_IS_WIN
#include <omega/filesystem_win.h>

OMG_API OMG_FileWin* omg_win_file_from_fp(OMG_Omega* this, OMG_FileWin* file, const OMG_String* path, int mode);
OMG_API bool omg_win_file_destroy(OMG_FileWin* file);
OMG_API int64_t omg_win_file_get_size(OMG_FileWin* file);
OMG_API int64_t omg_win_file_seek(OMG_FileWin* file, int64_t offset, int whence);
OMG_API size_t omg_win_file_read(OMG_FileWin* file, void* buf, size_t size, size_t maxnum);
OMG_API size_t omg_win_file_write(OMG_FileWin* file, const void* buf, size_t size, size_t num);
#endif
#endif
