#include <omega/omega.h>
#if OMG_HAS_STD
#include <stdio.h>
#endif
#include <omega/api_win.h>
#include <omega/omega_raylib.h>
#include <omega/omega_sdl2.h>
#include <omega/omega_win.h>
#include <omega/audio_fmod.h>
#include <omega/audio_sdl2.h>
#include <omega/audio_emscripten.h>
#include <omega/api_libc.h>
#define file_base ((OMG_File*)file)
#define file_omg ((OMG_Omega*)file_base->omg)
#define d_k32 ((OMG_Kernel32*)this->k32)
#define d_u32 ((OMG_User32*)this->u32)
#define d_libc ((OMG_Libc*)this->libc)
#define d_msvcrt ((OMG_Msvcrt*)this->msvcrt)

static OMG_Omega* omg_def_omega = NULL;

OMG_Omega* omg_get_default_omega(void) {
    return omg_def_omega;
}

OMG_Omega* omg_create(OMG_EntryData* data) {
    OMG_UNUSED(data);
    return NULL;
}

OMG_Omega* omg_create_by_type(OMG_EntryData* data, int omg_type) {
#if OMG_SUPPORT_SDL2
    if (omg_type == OMG_OMEGA_TYPE_SDL2)
        return (OMG_Omega*)omg_sdl2_create(data);
#endif
#if OMG_SUPPORT_WIN
    if (omg_type == OMG_OMEGA_TYPE_WIN)
        return (OMG_Omega*)omg_win_create(data);
#endif
#if OMG_SUPPORT_RAYLIB
    if (omg_type == OMG_OMEGA_TYPE_RAYLIB)
        return (OMG_Omega*)omg_raylib_create(data);
#endif
    return omg_create(data);
}

void omg_delay(OMG_Omega* this, double seconds) {
    OMG_UNUSED(this, seconds);
}

OMG_Omega* omg_alloc_omega_internal(OMG_EntryData* data, size_t size) {
    OMG_UNUSED(data, size);
    static uint8_t buf[1024 * 8];
    return (OMG_Omega*)buf;
}

void omg_free_omega_internal(OMG_Omega* this) {
    OMG_UNUSED(this);
}

void omg_fill_on_create(OMG_Omega* this, OMG_EntryData* data) {
    this->entry_data = data;
    this->mem = NULL;
    this->winmgr = NULL;
    this->audio = NULL;
    this->log_level = this->log_level_omg = this->log_level_lib = -1;
    this->sdl2_dll_path = NULL;
    this->sdl2_image_dll_path = NULL;
    this->sdl2_mixer_dll_path = NULL;
    this->sdl2_ttf_dll_path = NULL;
    this->fmod_dll_path = NULL;
    this->spng_dll_path = NULL;
    this->libc_dll_path = NULL;
    this->log_str_type = NULL;
#if OMG_IS_WIN
    this->k32 = NULL;
    this->dwm = NULL;
    this->uxtheme = NULL;
    this->g32 = NULL;
    this->nt = NULL;
#endif
#if OMG_SUPPORT_LIBC
    this->libc = NULL;
#endif
    this->support_highdpi = true;
    this->app_theme = OMG_THEME_AUTO;
    this->omg_init = omg_omg_init;
}

void omg_log_set_level(OMG_Omega* this, const int log_level, const int omg_log_level, const int lib_log_level) {
    if (log_level >= 0)
        this->log_level = log_level;
    if (omg_log_level >= 0)
        this->log_level_omg = omg_log_level;
    if (lib_log_level >= 0)
        this->log_level_lib = lib_log_level;
}

bool omg_log_str_type(OMG_Omega* this, const OMG_String* data, int type) {
    OMG_UNUSED(this, data, type);
    return false;
}

bool omg_destroy(OMG_Omega* this) {
    omg_free_omega_internal(this);
    omg_def_omega = NULL;
    return false;
}

bool omg_app_init(OMG_Omega* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_alloc_winmgr(OMG_Omega* this) {
    if (OMG_ISNULL(this->winmgr))
        return false;
    this->winmgr->omg = this;
    this->winmgr->was_allocated = true;
    this->winmgr->inited = false;
    this->winmgr->init = omg_winmgr_init;
    return false;
}

bool omg_free_winmgr(OMG_Omega* this) {
    if (OMG_ISNULL(this->winmgr))
        return false;
    if (this->winmgr->inited)
        this->winmgr->destroy(this->winmgr);
    if (this->winmgr->was_allocated) {
        OMG_FREE(this->mem, this->winmgr);
        this->winmgr = NULL;
    }
    return false;
}

bool omg_app_quit(OMG_Omega* this) {
    omg_free_winmgr(this);
    if (OMG_ISNOTNULL(this->clock) && this->clock->was_allocated) {
        if (this->clock->inited)
            this->clock->destroy(this->clock);
        OMG_FREE(this->mem, this->clock);
        this->clock = NULL;
    }
    if (OMG_ISNOTNULL(this->keyboard_state)) {
        OMG_FREE(this->mem, this->keyboard_state);
        this->keyboard_state = NULL;
        this->keymap = NULL;
    }
    this->audio_free(this);
    return false;
}

void omg_auto_loop_run(OMG_Omega* this) {
    this->looping = false;
}

void omg_auto_loop_stop(OMG_Omega* this) {
    this->looping = false;
}

int omg_set_text_input_state(OMG_Omega* this, int state) {
    OMG_UNUSED(this, state);
    return OMG_TEXT_INPUT_DISABLED;
}

void omg_event_on_quit(OMG_EventQuit* event) {
    OMG_Omega* this = OMG_OMEGA_FROM_EVENT(event);
    this->auto_loop_stop(this);
}

void omg_event_on_update(OMG_EventUpdate* event) {
    OMG_UNUSED(event);
}

void omg_event_on_paint(OMG_EventPaint* event) {
    OMG_UNUSED(event);
}

void omg_event_on_expose(OMG_EventExpose* event) {
    OMG_UNUSED(event);
}

void omg_event_on_resize(OMG_EventResize* event) {
    OMG_UNUSED(event);
}

void omg_event_on_loop_stop(OMG_EventLoopStop* event) {
    OMG_UNUSED(event);
}

void omg_event_on_state_change(OMG_EventStateChange* event) {
    OMG_UNUSED(event);
}

void omg_event_on_focus_change(OMG_EventFocusChange* event) {
    OMG_UNUSED(event);
}

void omg_event_on_mouse_wheel(OMG_EventMouseWheel* event) {
    OMG_UNUSED(event);
}

void omg_event_on_state_changing(OMG_EventStateChanging* event) {
    OMG_Window* this = OMG_WIN_FROM_EVENT(event);
    if (event->change == OMG_WIN_STATE_CLOSED) {
        if (!(this->sys_buttons & OMG_WIN_SYS_BUTTON_CLOSE))
            event->allow = false;
    }
    else if (event->change == OMG_WIN_STATE_MAXIMIZED) {
        if (!(this->sys_buttons & OMG_WIN_SYS_BUTTON_MAXIMIZE))
            event->allow = false;
    }
    else if (event->change == OMG_WIN_STATE_MINIMIZED) {
        if (!(this->sys_buttons & OMG_WIN_SYS_BUTTON_MINIMIZE))
            event->allow = false;
    }
}

void omg_event_on_close(OMG_EventClose* event) {
    OMG_UNUSED(event);
}

void omg_event_on_mouse_move(OMG_EventMouseMove* event) {
    OMG_UNUSED(event);
}

void omg_event_on_mouse_button(OMG_EventMouseButton* event) {
    OMG_UNUSED(event);
}

void omg_event_on_mouse_focus(OMG_EventMouseFocus* event) {
    OMG_UNUSED(event);
}

void omg_event_on_keyboard(OMG_EventKeyboard* event) {
    OMG_UNUSED(event);
}

void omg_event_on_text_input(OMG_EventTextInput* event) {
    OMG_UNUSED(event);
}

void omg_event_on_touch(OMG_EventTouch* event) {
    OMG_UNUSED(event);
}

void omg_reset_event_handlers(OMG_Omega* this) {
    this->on_quit = omg_event_on_quit;
    this->on_update = omg_event_on_update;
    this->on_loop_stop = omg_event_on_loop_stop;
    this->on_paint = omg_event_on_paint;
    this->on_expose = omg_event_on_expose;
    this->on_close = omg_event_on_close;
    this->on_resize = omg_event_on_resize;
    this->on_size_change = omg_event_on_resize;
    this->on_state_change = omg_event_on_state_change;
    this->on_state_changing = omg_event_on_state_changing;
    this->on_mouse_move = omg_event_on_mouse_move;
    this->on_mouse_down = omg_event_on_mouse_button;
    this->on_mouse_up = omg_event_on_mouse_button;
    this->on_mouse_wheel = omg_event_on_mouse_wheel;
    this->on_mouse_enter = omg_event_on_mouse_focus;
    this->on_mouse_leave = omg_event_on_mouse_focus;
    this->on_focus_change = omg_event_on_focus_change;
    this->on_key_down = omg_event_on_keyboard;
    this->on_key_up = omg_event_on_keyboard;
    this->on_text_input = omg_event_on_text_input;
    this->on_touch_down = omg_event_on_touch;
    this->on_touch_up = omg_event_on_touch;
    this->on_touch_move = omg_event_on_touch;
}

bool omg_file_destroy(OMG_File* file) {
    omg_string_destroy(&file->fp);
    if (file->was_allocated) {
        file->was_allocated = false;
        OMG_FREE(((OMG_Omega*)file->omg)->mem, file);
    }
    return false;
}

int64_t omg_file_get_size(OMG_File* file) {
    OMG_UNUSED(file);
    return -1;
}

int64_t omg_file_seek(OMG_File* file, int64_t offset, int whence) {
    OMG_UNUSED(file, offset, whence);
    return -1;
}

int64_t omg_file_tell(OMG_File* file) {
    return file->seek(file, 0, OMG_FILE_SEEK_CUR);
}

size_t omg_file_read(OMG_File* file, void* buf, size_t size, size_t maxnum) {
    OMG_UNUSED(file, buf, size, maxnum);
    return 0;
}

size_t omg_file_write(OMG_File* file, const void* buf, size_t size, size_t num) {
    OMG_UNUSED(file, buf, size, num);
    return 0;
}

OMG_File* omg_file_from_fp(OMG_Omega* this, OMG_File* file, const OMG_String* path, int mode) {
    if (OMG_ISNULL(file)) {
        file = OMG_MALLOC(this->mem, this->sz_file);
        if (OMG_ISNULL(file))
            return NULL;
        file->was_allocated = true;
    }
    else
        file->was_allocated = false;
    file->omg = this;
    if (omg_string_init_dynamic(&file->fp, path)) {
        OMG_FREE(this->mem, file);
        return NULL;
    }
    file->mode = mode;
    file->type = OMG_FILE_TYPE_UNKNOWN;
    file->destroy = omg_file_destroy;
    file->get_size = omg_file_get_size;
    file->seek = omg_file_seek;
    file->tell = omg_file_tell;
    file->read = omg_file_read;
    file->write = omg_file_write;
    return file;
}

// Thanks to SDL2
int64_t omg_file_mem_get_size(OMG_FileMem* file) {
    return (int64_t)(file->stop - file->base);
}

int64_t omg_file_mem_seek(OMG_FileMem* file, int64_t offset, int whence) {
    uint8_t* newpos;
    if (whence == OMG_FILE_SEEK_END)
        newpos = file->stop + offset;
    else if (whence == OMG_FILE_SEEK_CUR)
        newpos = file->here + offset;
    else
        newpos = file->base + offset;
    if (newpos < file->base)
        newpos = file->base;
    if (newpos > file->stop)
        newpos = file->stop;
    file->here = newpos;
    return (int64_t)(file->here - file->base);
}

int64_t omg_file_mem_tell(OMG_FileMem* file) {
    return (int64_t)(file->here - file->base);
}

static size_t omg_file_mem_io(OMG_FileMem* file, void* dst, const void* src, size_t size) {
    const size_t mem_available = (size_t)(file->stop - file->here);
    if (size > mem_available)
        size = mem_available;
    file_omg->std->memcpy(dst, src, size);
    file->here += size;
    return size;
}

size_t omg_file_mem_read(OMG_FileMem* file, void* buf, size_t size, size_t maxnum) {
    return omg_file_mem_io(file, buf, file->here, size * maxnum);
}

size_t omg_file_mem_write(OMG_FileMem* file, const void* buf, size_t size, size_t num) {
    if (file_base->type == OMG_FILE_TYPE_MEMORY_RO)
        return 0;
    return omg_file_mem_io(file, file->here, buf, size * num);
}

OMG_FileMem* omg_file_from_mem(OMG_Omega* this, OMG_FileMem* file, const void* mem, size_t size, bool read_only) {
    if (OMG_ISNULL(file)) {
        file = OMG_MALLOC(this->mem, sizeof(OMG_FileMem));
        if (OMG_ISNULL(file))
            return NULL;
        file_base->was_allocated = true;
    }
    else
        file_base->was_allocated = false;
    file_base->omg = this;
    file_base->type = read_only ? OMG_FILE_TYPE_MEMORY_RO : OMG_FILE_TYPE_MEMORY;
    OMG_BEGIN_POINTER_CAST();
    file_base->get_size = omg_file_mem_get_size;
    file_base->seek = omg_file_mem_seek;
    file_base->tell = omg_file_mem_tell;
    file_base->read = omg_file_mem_read;
    file_base->write = omg_file_mem_write;
    OMG_END_POINTER_CAST();
    file->base = (uint8_t*)mem;
    file->here = file->base;
    file->stop = file->base + size;
    return file;
}

#if OMG_HAS_STD
// TODO: move win32 api here like this
// TODO: support 64-bit types
bool omg_file_std_destroy(OMG_FileStd* file) {
    bool res = false;
    FILE* handle = (FILE*)file->file;
    file->file = NULL;
    OMG_BEGIN_POINTER_CAST();
    omg_file_destroy(file);
    OMG_END_POINTER_CAST();
    if (OMG_ISNOTNULL(handle)) {
        if (fclose(handle) != 0)
            res = true;
    }
    return res;
}

int64_t omg_file_std_get_size(OMG_FileStd* file) {
    long pos = ftell(file->file);
    if (fseek(file->file, 0, SEEK_END) != 0) {
        _OMG_LOG_WARN(file_omg, "Failed to get size for std file ", file_base->fp.ptr);
        return -2;
    }
    long res = ftell(file->file);
    fseek(file->file, pos, SEEK_SET);
    if (res < 0)
        _OMG_LOG_WARN(file_omg, "Failed to get size for std file ", file_base->fp.ptr);
    if (res == -1)
        res = -2;
    return (int64_t)res;
}

int64_t omg_file_std_seek(OMG_FileStd* file, int64_t offset, int whence) {
    if (fseek(file->file, (long)offset, (
        (whence == OMG_FILE_SEEK_END) ? SEEK_END : ((whence == OMG_FILE_SEEK_CUR) ? SEEK_CUR : SEEK_SET)
    )) != 0) {
        _OMG_LOG_WARN(file_omg, "Failed to seek std file ", file_base->fp.ptr);
        return -2;
    }
    long res = ftell(file->file);
    if (res < 0) {
        _OMG_LOG_WARN(file_omg, "Failed to get pos for std file ", file_base->fp.ptr);
        res = -1;
    }
    return (int64_t)res;
}

int64_t omg_file_std_tell(OMG_FileStd* file) {
    long res = ftell(file->file);
    if (res < 0) {
        _OMG_LOG_WARN(file_omg, "Failed to get pos for std file ", file_base->fp.ptr);
        res = -1;
    }
    return (int64_t)res;
}

size_t omg_file_std_read(OMG_FileStd* file, void* buf, size_t size, size_t maxnum) {
    size_t res = fread(buf, size, maxnum, file->file);
    if ((res < (size * maxnum)) && !feof(((FILE*)file->file))) {
        _OMG_LOG_WARN(file_omg, "Failed to read std file ", file_base->fp.ptr);
    }
    return res;
}

size_t omg_file_std_write(OMG_FileStd* file, const void* buf, size_t size, size_t num) {
    size_t res = fwrite(buf, size, num, file->file);
    if (res < (size * num)) {
        _OMG_LOG_WARN(file_omg, "Failed to write std file ", file_base->fp.ptr);
    }
    return res;
}

OMG_FileStd* omg_file_std_from_path(OMG_Omega* this, OMG_FileStd* file, const OMG_String* path, int mode) {
    const char* str_mode = NULL;
    _OMG_FILE_MODE_TO_STD(mode, str_mode);
    if (OMG_ISNULL(str_mode)) {
        _OMG_LOG_ERROR(this, "Invalid mode for opening file ", path->ptr);
        return NULL;
    }
    OMG_BEGIN_POINTER_CAST();
    if (omg_string_ensure_null((OMG_String*)path))
        return NULL;
    file = omg_file_from_fp(this, file, path, mode);
    if (OMG_ISNULL(file))
        return NULL;
#if OMG_IS_VC // Fuck Visual Studio
    // TODO: use UTF-8 mode. Example: "w+, ccs=UTF-8"
    int res = fopen_s(&file->file, path->ptr, str_mode);
#else
    int res = 0;
    file->file = fopen(path->ptr, str_mode);
#endif
    if ((res != 0) || OMG_ISNULL(file->file)) {
        _OMG_LOG_ERROR(this, "Failed to std open file ", path->ptr);
        omg_file_destroy(file);
        return NULL;
    }
    file_base->type = OMG_FILE_TYPE_STDFILE;
    file_base->destroy = omg_file_std_destroy;
    file_base->get_size = omg_file_std_get_size;
    file_base->seek = omg_file_std_seek;
    file_base->tell = omg_file_std_tell;
    file_base->read = omg_file_std_read;
    file_base->write = omg_file_std_write;
    OMG_END_POINTER_CAST();
    return file;
}
#endif

bool omg_dummy_audio_alloc(OMG_Omega* this) {
#if OMG_ALLOW_DUMMY_AUDIO
    if (OMG_ISNULL(this->audio)) {
        static OMG_Audio audio;
        this->audio = &audio;
    }
    this->audio->was_allocated = false;
    this->audio->omg = this;
    omg_audio_fill_on_create(this->audio);
    return false;
#else
    OMG_UNUSED(this);
    return true;
#endif
}

bool omg_audio_alloc(OMG_Omega* this) {
    OMG_BEGIN_POINTER_CAST();
#if OMG_SUPPORT_EMSCRIPTEN_AUDIO
    if ((this->audio_type == OMG_AUDIO_TYPE_FMOD) || (this->audio_type == OMG_AUDIO_FORMAT_AUTO)) {
        if (OMG_ISNULL(this->audio)) {
            this->audio = OMG_MALLOC(this->mem, sizeof(OMG_AudioEm));
            if (OMG_ISNULL(this->audio))
                return omg_dummy_audio_alloc(this);
            this->audio->was_allocated = true;
        }
        else
            this->audio->was_allocated = false;
        omg_audio_fill_on_create(this->audio);
        this->audio->omg = this;
        this->audio->init = omg_audio_emscripten_init;
        return false;
    }
#endif
#if OMG_SUPPORT_SDL2_MIXER
    if (this->audio_type == OMG_AUDIO_TYPE_SDL2) {
        if (OMG_ISNULL(this->audio)) {
            this->audio = OMG_MALLOC(this->mem, sizeof(OMG_AudioSdl2) + sizeof(OMG_Sdl2));
            if (OMG_ISNULL(this->audio))
                return omg_dummy_audio_alloc(this);
            this->audio->was_allocated = true;
        }
        else
            this->audio->was_allocated = false;
        omg_audio_fill_on_create(this->audio);
        this->audio->omg = this;
        ((OMG_AudioSdl2*)this->audio)->sdl2 = (OMG_Sdl2*)((size_t)this->audio + sizeof(OMG_AudioSdl2)); // Hack
        if (omg_sdl2_dll_load(((OMG_AudioSdl2*)this->audio)->sdl2, this->sdl2_mixer_dll_path)) { // TODO: free SDL2
            OMG_FREE(this->mem, this->audio);
            this->audio = NULL;
            return true;
        }
        this->audio->init = omg_audio_sdl2_init;
        return false;
    }
#endif
#if OMG_SUPPORT_FMOD
    this->audio_type = OMG_AUDIO_TYPE_FMOD; // Hack
    if (this->audio_type == OMG_AUDIO_TYPE_FMOD) {
        if (OMG_ISNULL(this->audio)) {
            this->audio = OMG_MALLOC(this->mem, sizeof(OMG_AudioFmod));
            if (OMG_ISNULL(this->audio))
                return omg_dummy_audio_alloc(this);
            this->audio->was_allocated = true;
        }
        else
            this->audio->was_allocated = false;
        omg_audio_fill_on_create(this->audio);
        this->audio->omg = this;
        this->audio->init = omg_audio_fmod_init;
        return false;
    }
#endif
    OMG_END_POINTER_CAST();
    return omg_dummy_audio_alloc(this);
}

bool omg_audio_free(OMG_Omega* this) {
    if (OMG_ISNULL(this->audio))
        return false;
    if (this->audio->inited)
        this->audio->destroy(this->audio);
    if (this->audio->was_allocated) {
        this->audio->was_allocated = false;
        OMG_FREE(this->mem, this->audio);
        this->audio = NULL;
    }
    return false;    
}

bool omg_win_destroy_clean1(OMG_Omega* this) {
#if OMG_IS_WIN
    bool result = false;
    if (this->should_free_msvcrt && OMG_ISNOTNULL(this->msvcrt)) {
        result = omg_winapi_msvcrt_free(this->msvcrt) || result;
        result = OMG_FREE(this->mem, this->msvcrt) || result;
        this->msvcrt = NULL;
    }
    if (this->should_free_g32 && OMG_ISNOTNULL(this->g32)) {
        result = omg_winapi_gdi32_free(this->g32) || result;
        result = OMG_FREE(this->mem, this->g32) || result;
        this->g32 = NULL;
    }
    if (this->should_free_u32 && OMG_ISNOTNULL(this->u32)) {
        result = omg_winapi_user32_free(this->u32) || result;
        result = OMG_FREE(this->mem, this->u32) || result;
        this->u32 = NULL;
    }
    if (this->should_free_uxtheme && OMG_ISNOTNULL(this->uxtheme)) {
        result = omg_winapi_uxtheme_free(this->uxtheme) || result;
        result = OMG_FREE(this->mem, this->uxtheme) || result;
        this->uxtheme = NULL;
    }
    if (this->should_free_dwm && OMG_ISNOTNULL(this->dwm)) {
        result = omg_winapi_dwmapi_free(this->dwm) || result;
        result = OMG_FREE(this->mem, this->dwm) || result;
        this->dwm = NULL;
    }
    if (this->should_free_ntdll && OMG_ISNOTNULL(this->dwm)) {
        result = omg_winapi_ntdll_free(this->nt) || result;
        result = OMG_FREE(this->mem, this->nt) || result;
        this->nt = NULL;
    }
    return result;
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_win_destroy_clean2(OMG_Omega* this) {
#if OMG_IS_WIN
    bool result = false;
    if (this->should_free_k32 && OMG_ISNOTNULL(this->k32)) {
        result = omg_winapi_kernel32_free(this->k32) || result;
        this->k32 = NULL;
    }
    return result;
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_win_loads_libs1(OMG_Omega* this) {
#if OMG_IS_WIN
    if (OMG_ISNULL(this->k32)) {
        this->k32 = &this->k32_stk;
        this->should_free_k32 = false;
        if (omg_winapi_kernel32_load(this->k32))
            return true;
    }
    return false;
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_win_loads_libs2(OMG_Omega* this) {
#if OMG_IS_WIN
    if (OMG_ISNULL(this->nt)) {
        this->nt = OMG_MALLOC(this->mem, sizeof(OMG_Ntdll));
        if (OMG_ISNULL(this->nt) || omg_winapi_ntdll_load(this->nt)) {
            omg_win_destroy_clean1(this);
            if (OMG_ISNOTNULL(this->nt)) {
                OMG_FREE(this->mem, this->nt);
            }
            this->mem->destroy(this->mem);
            this->mem = NULL;
            omg_win_destroy_clean2(this);
            return true;
        }
        this->should_free_ntdll = true;
    }
    OMG_WIN_NTDLL_OSVERSIONINFOEXW os_ver_info;
    os_ver_info.dwOSVersionInfoSize = sizeof(OMG_WIN_NTDLL_OSVERSIONINFOEXW);
    ((OMG_Ntdll*)(this->nt))->RtlGetVersion(&os_ver_info);
    this->win_major_ver = (int)os_ver_info.dwMajorVersion;
    this->win_minor_ver = (int)os_ver_info.dwMinorVersion;
    this->win_build_number = (int)os_ver_info.dwBuildNumber;
    return false;
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_win_loads_libs3(OMG_Omega* this) {
#if OMG_IS_WIN
    if (OMG_ISNULL(this->dwm)) {
        this->dwm = OMG_MALLOC(this->mem, sizeof(OMG_Dwmapi));
        if (OMG_ISNULL(this->dwm)) {
            return true;
        }
        omg_winapi_dwmapi_load(this->dwm);
        this->should_free_dwm = true;
    }
    if (OMG_ISNULL(this->uxtheme)) {
        this->uxtheme = OMG_MALLOC(this->mem, sizeof(OMG_Uxtheme));
        if (OMG_ISNULL(this->uxtheme)) {
            return true;
        }
        omg_winapi_uxtheme_load(this->uxtheme, this->win_build_number);
        this->should_free_uxtheme = true;
    }
    if (OMG_ISNULL(this->u32)) {
        this->u32 = OMG_MALLOC(this->mem, sizeof(OMG_User32));
        if (OMG_ISNULL(this->u32)) {
            return true;
        }
        if (omg_winapi_user32_load(this->u32)) {
            OMG_FREE(this->mem, this->u32);
            return true;
        }
        this->should_free_u32 = true;
    }
    if (OMG_ISNULL(this->g32)) {
        this->g32 = OMG_MALLOC(this->mem, sizeof(OMG_Gdi32));
        if (OMG_ISNULL(this->g32)) {
            return true;
        }
        if (omg_winapi_gdi32_load(this->g32)) {
            OMG_FREE(this->mem, this->g32);
            return true;
        }
        this->should_free_g32 = true;
    }
    if (OMG_ISNULL(this->msvcrt)) {
        this->msvcrt = OMG_MALLOC(this->mem, sizeof(OMG_Msvcrt));
        if (OMG_ISNULL(this->msvcrt)) {
            return true;
        }
        if (omg_winapi_msvcrt_load(this->msvcrt)) {
            d_msvcrt->_beginthreadex = NULL;
            d_msvcrt->_endthreadex = NULL;
            d_msvcrt->handle = NULL;
            // OMG_FREE(this->mem, this->msvcrt);
            // return true;
        }
        this->should_free_msvcrt = true;
    }
    return false;
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_fs_is_file_or_dir(OMG_Omega* this, const OMG_String* path, int type) {
#if OMG_IS_WIN
    if (OMG_ISNULL(d_k32->GetFileAttributesW))
        return false;
    size_t count;
    _OMG_WIN_GET_ENCODE_SIZE(count, path, d_k32);
    if (count == 0) {
        _OMG_LOG_ERROR(this, "Failed to check Win32 file ", path);
        return false;
    }
    wchar_t* w_fp = OMG_MALLOC(this->mem, (size_t)count * 2 + 2);
    if (OMG_ISNULL(w_fp)) {
        _OMG_LOG_ERROR(this, "Failed to check Win32 file ", path);
        return false;
    }
    int out_len = d_k32->MultiByteToWideChar(CP_UTF8, 0, path->ptr, (int)path->len, w_fp, (int)count);
    if (out_len > 0)
        w_fp[out_len] = L'\0';
    DWORD res = d_k32->GetFileAttributesW(w_fp);
    OMG_FREE(this->mem, w_fp);
    if (type == 0)
        return (res != INVALID_FILE_ATTRIBUTES) && !(res & FILE_ATTRIBUTE_DIRECTORY);
    if (type == 1)
        return (res & FILE_ATTRIBUTE_DIRECTORY) > 0;
    return (res != INVALID_FILE_ATTRIBUTES);
#elif OMG_SUPPORT_LIBC
    if (omg_string_ensure_null((OMG_String*)path))
        return false;
    struct stat stat_buf;
    if (d_libc->stat(path->ptr, &stat_buf) != 0)
        return false;
    if (type == 0)
        return !S_ISDIR(stat_buf.st_mode);
    else if (type == 1)
        return S_ISDIR(stat_buf.st_mode);
    return true;
#else
    OMG_UNUSED(this, path, type);
    return false;
#endif
}

bool omg_fs_remove_file_or_dir(OMG_Omega* this, const OMG_String* path, int type) {
#if OMG_IS_WIN
    if (OMG_ISNULL(d_k32->RemoveDirectoryW) || OMG_ISNULL(d_k32->DeleteFileW))
        return true;
    if (!this->fs_is_file_or_dir(this, path, (type == 2) ? 1 : type))
        return true;
    size_t count;
    _OMG_WIN_GET_ENCODE_SIZE(count, path, d_k32);
    if (count == 0) {
        _OMG_LOG_ERROR(this, "Failed to remove Win32 path ", path);
        return true;
    }
    wchar_t* w_fp = OMG_MALLOC(this->mem, (size_t)count * 2 + 2);
    if (OMG_ISNULL(w_fp)) {
        _OMG_LOG_ERROR(this, "Failed to remove Win32 path ", path);
        return true;
    }
    int out_len = d_k32->MultiByteToWideChar(CP_UTF8, 0, path->ptr, (int)path->len, w_fp, (int)count);
    if (out_len > 0)
        w_fp[out_len] = L'\0';
    bool res = false;
    if (type == 0)
        res = !d_k32->DeleteFileW(w_fp);
    else if ((type == 1) || (type == 2))
        res = !d_k32->RemoveDirectoryW(w_fp);
    OMG_FREE(this->mem, w_fp);
    return res;
#elif OMG_SUPPORT_LIBC
    if (omg_string_ensure_null((OMG_String*)path))
        return true;
    if (type == 0) {
        if (this->fs_is_file_or_dir(this, path, 0))
            return (d_libc->remove(path->ptr) != 0);
        return true;
    }
    if (type == 1) {
        if (this->fs_is_file_or_dir(this, path, 1))
            return (d_libc->rmdir(path->ptr) != 0);
        return true;
    }
    return true;
#else
    OMG_UNUSED(this, path, type);
    return true;
#endif
}

bool omg_fs_move(OMG_Omega* this, const OMG_String* old_path, const OMG_String* new_path) {
#if OMG_IS_WIN
    if (OMG_ISNULL(d_k32->MoveFileW))
        return false;
    size_t count1;
    size_t count2;
    _OMG_WIN_GET_ENCODE_SIZE(count1, old_path, d_k32);
    _OMG_WIN_GET_ENCODE_SIZE(count2, new_path, d_k32);
    if ((count1 == 0) || (count2 == 0)) {
        return true;
    }
    wchar_t* w_o_fp = OMG_MALLOC(this->mem, (size_t)(count1 + count2) * 2 + 4);
    if (OMG_ISNULL(w_o_fp))
        return true;
    wchar_t* w_n_fp = (wchar_t*)((size_t)w_o_fp + ((size_t)(count1) * 2) + 2);
    int out_len_o = d_k32->MultiByteToWideChar(CP_UTF8, 0, old_path->ptr, (int)old_path->len, w_o_fp, (int)count1);
    int out_len_n = d_k32->MultiByteToWideChar(CP_UTF8, 0, new_path->ptr, (int)new_path->len, w_n_fp, (int)count2);
    if (out_len_o > 0)
        w_o_fp[out_len_o] = L'\0';
    if (out_len_n > 0)
        w_n_fp[out_len_n] = L'\0';
    bool res = !d_k32->MoveFileW(w_o_fp, w_n_fp);
    OMG_FREE(this->mem, w_o_fp);
    return res;
#elif OMG_SUPPORT_LIBC
    if (omg_string_ensure_null((OMG_String*)old_path) || omg_string_ensure_null((OMG_String*)new_path))
        return true;
    return (d_libc->rename(old_path->ptr, new_path->ptr) != 0);
#else
    OMG_UNUSED(this, old_path, new_path);
    return true;
#endif
}

bool omg_libc_init(OMG_Omega* this) {
#if OMG_SUPPORT_LIBC
    if (OMG_ISNULL(this->libc)) {
        this->libc = OMG_MALLOC(this->mem, sizeof(OMG_Libc));
        if (OMG_ISNULL(this->libc))
            return true;
        if (omg_libc_dll_load(this->libc, this->libc_dll_path)) {
            OMG_FREE(this->mem, this->libc);
            return true;
        }
        this->should_free_libc = true;
    }
    return false;
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_libc_destroy(OMG_Omega* this) {
#if OMG_SUPPORT_LIBC
    if (OMG_ISNULL(this->libc))
        return false;
    bool res = false;
    if (this->should_free_libc) {
        this->should_free_libc = false;
        res = omg_libc_dll_free(this->libc);
        OMG_FREE(this->mem, this->libc);
    }
    return res;
#else
    OMG_UNUSED(this);
    return false;
#endif
}

OMG_String omg_env_get(OMG_Omega* this, const OMG_String* key_name) {
#if OMG_IS_WIN
    // Fuck Microsoft
    size_t count;
    _OMG_WIN_GET_ENCODE_SIZE(count, key_name, d_k32);
    if (count == 0)
        return *omg_dummy_string_create();
    wchar_t* w_fp = OMG_MALLOC(this->mem, (size_t)count * 2 + 2);
    if (OMG_ISNULL(w_fp))
        return *omg_dummy_string_create();
    int out_len = d_k32->MultiByteToWideChar(CP_UTF8, 0, key_name->ptr, (int)key_name->len, w_fp, (int)count);
    if (out_len > 0)
        w_fp[out_len] = L'\0';
    // Fuck Microsoft
    // size_t need_len = (size_t)d_k32->GetEnvironmentVariableW(w_fp, buf123, 1);
    const size_t need_len = 1024 * 10;
    // TODO: Where does leak peace of microsoft's shit here when I alloc with OMG_MALLOC???!!!
    wchar_t buf[1024 * 10];
    if ((need_len <= 1) || OMG_ISNULL(buf)) {
        if (OMG_ISNOTNULL(buf))
            OMG_FREE(this->mem, buf);
        return *omg_dummy_string_create();
    }
    DWORD read_len = d_k32->GetEnvironmentVariableW(w_fp, buf, (DWORD)(need_len));
    OMG_FREE(this->mem, w_fp);
    if (read_len == 0)
        return *omg_dummy_string_create();
    buf[read_len] = L'\0';
    OMG_String res;
    if (omg_string_init_dynamic(&res, NULL)) {
        OMG_FREE(this->mem, buf);
        return *omg_dummy_string_create();
    }
    if (omg_string_add_wchar_p(&res, buf)) {
        omg_string_destroy(&res);
        OMG_FREE(this->mem, buf);
        return *omg_dummy_string_create();
    }
    return res;
#elif OMG_SUPPORT_LIBC
    OMG_UNUSED(this, key_name);
    return *omg_dummy_string_create();
#else
    OMG_UNUSED(this, key_name);
    return *omg_dummy_string_create();
#endif
}

bool omg_env_set(OMG_Omega* this, const OMG_String* key_name, const OMG_String* key_value, bool overwrite) {
#if OMG_IS_WIN
    if (!overwrite) {
        OMG_String temp_res = this->env_get(this, key_name);
        if (temp_res.len > 0) {
            omg_string_destroy(&temp_res);
            return false;
        }
    }
    size_t count1;
    size_t count2;
    _OMG_WIN_GET_ENCODE_SIZE(count1, key_name, d_k32);
    if (OMG_ISNULL(key_value))
        count2 = 0;
    else
        _OMG_WIN_GET_ENCODE_SIZE(count2, key_value, d_k32);
    if (count1 == 0) {
        return true;
    }
    wchar_t* w_o_fp = OMG_MALLOC(this->mem, (size_t)(count1 + count2) * 2 + 4);
    if (OMG_ISNULL(w_o_fp))
        return true;
    wchar_t* w_n_fp = (wchar_t*)((size_t)w_o_fp + ((size_t)(count1) * 2) + 2);
    int out_len_o = d_k32->MultiByteToWideChar(CP_UTF8, 0, key_name->ptr, (int)key_name->len, w_o_fp, (int)count1);
    if (out_len_o > 0)
        w_o_fp[out_len_o] = L'\0';
    if (OMG_ISNOTNULL(key_value)) {
        int out_len_n = d_k32->MultiByteToWideChar(CP_UTF8, 0, key_value->ptr, (int)key_value->len, w_n_fp, (int)count2);
        if (out_len_n > 0)
            w_n_fp[out_len_n] = L'\0';
    }
    bool res = !d_k32->SetEnvironmentVariableW(w_o_fp, OMG_ISNULL(key_value) ? NULL : w_n_fp);
    OMG_FREE(this->mem, w_o_fp);
    return res;
#elif OMG_SUPPORT_LIBC
    OMG_UNUSED(this, key_name, key_value, overwrite);
    return true;
#else
    OMG_UNUSED(this, key_name, key_value, overwrite);
    return true;
#endif
}

bool omg_message_box(OMG_Omega* this, const OMG_String* text, const OMG_String* title, int flags) {
#if OMG_IS_WIN
    size_t count1;
    size_t count2;
    char* title_ptr;
    _OMG_MSGBOX_DEF_FILL_TITLE(title, title_ptr);
    size_t title_ptr_len = OMG_ISNULL(title) ? this->std->strlen(title_ptr) : title->len;
    _OMG_WIN_GET_ENCODE_SIZE(count1, text, d_k32);
    count2 = title_ptr_len * 2;
    if (count1 == 0) {
        return true;
    }
    wchar_t* w_o_fp = OMG_MALLOC(this->mem, (size_t)(count1 + count2) * 2 + 4);
    if (OMG_ISNULL(w_o_fp))
        return true;
    wchar_t* w_n_fp = (wchar_t*)((size_t)w_o_fp + ((size_t)(count1) * 2) + 2);
    int out_len_o = d_k32->MultiByteToWideChar(CP_UTF8, 0, text->ptr, (int)text->len, w_o_fp, (int)count1);
    if (out_len_o > 0)
        w_o_fp[out_len_o] = L'\0';
    int out_len_n = d_k32->MultiByteToWideChar(CP_UTF8, 0, title_ptr, (int)title_ptr_len, w_n_fp, (int)count2);
    if (out_len_n > 0)
        w_n_fp[out_len_n] = L'\0';
    UINT win_flags = 0;
    if (flags & OMG_MESSAGEBOX_INFO)
        win_flags |= MB_ICONINFORMATION;
    if (flags & OMG_MESSAGEBOX_WARN)
        win_flags |= MB_ICONWARNING;
    if (flags & OMG_MESSAGEBOX_ERROR)
        win_flags |= MB_ICONERROR;
    bool res = false;
    if (d_u32->MessageBoxW(NULL, w_o_fp, w_n_fp, win_flags) == 0) {
        res = true;
    }
    OMG_FREE(this->mem, w_o_fp);
    return res;
#else
    OMG_UNUSED(this, text, title, flags);
    return true;
#endif
}

#if OMG_IS_WIN && OMG_SUPPORT_THREADING
typedef struct {
    HANDLE handle;
    OMG_Omega* omg;
    void* user_data;
    OMG_ThreadFunction func;
    uint32_t id;
    int status;
    bool running;
    bool should_free;
} OMG_ThreadWin;

static DWORD WINAPI OMG_MINGW32_FORCEALIGN omg_thread_run_with_create_thread(LPVOID data) {
    OMG_ThreadWin* thread = (OMG_ThreadWin*)data;
    OMG_Omega* this = thread->omg;
    thread->running = true;
    thread->status = thread->func(thread->user_data);
    thread->running = false;
    if (thread->should_free)
        OMG_FREE(this->mem, thread);
    return 0;
}

static unsigned __stdcall OMG_MINGW32_FORCEALIGN omg_thread_run_with_begin_thread_ex(void* data) {
    OMG_ThreadWin* thread = (OMG_ThreadWin*)data;
    OMG_Omega* this = thread->omg;
    thread->running = true;
    thread->status = thread->func(thread->user_data);
    thread->running = false;
    if (thread->should_free)
        OMG_FREE(this->mem, thread);
    d_msvcrt->_endthreadex(0);
    return 0;
}
#endif

OMG_Thread* omg_thread_create(OMG_Omega* this, OMG_ThreadFunction func, const OMG_String* name, void* data, size_t stack_size, void* reserved1, void* reserved2) {
#if OMG_IS_WIN && OMG_SUPPORT_THREADING
    OMG_UNUSED(this, func, name, data, stack_size, reserved1, reserved2);
    OMG_ThreadWin* thread = OMG_MALLOC(this->mem, sizeof(OMG_ThreadWin));
    if (OMG_ISNULL(thread))
        return NULL;
    const DWORD flags = (stack_size > 0) ? STACK_SIZE_PARAM_IS_A_RESERVATION : 0;
    thread->status = -1;
    thread->omg = this;
    thread->func = func;
    thread->user_data = data;
    thread->running = true;
    thread->should_free = false;
    if (OMG_ISNULL(d_msvcrt->_beginthreadex)) {
        DWORD threadid = 0;
        thread->handle = d_k32->CreateThread(NULL, stack_size, omg_thread_run_with_create_thread, thread, flags, &threadid);
        thread->id = (uint32_t)threadid;
    } else {
        unsigned threadid = 0;
        thread->handle = (HANDLE)((size_t)d_msvcrt->_beginthreadex(NULL, (unsigned int)stack_size, omg_thread_run_with_begin_thread_ex, thread, flags, &threadid));
        thread->id = (uint32_t)threadid;
    }
    if (OMG_ISNULL(thread->handle)) {
        // _OMG_LOG_ERROR(this, "Failed to create Win32 thread");
        OMG_FREE(this->mem, thread);
        return NULL;
    }
    return (OMG_Thread*)thread;
#else
    OMG_UNUSED(this, func, name, data, stack_size, reserved1, reserved2);
    return NULL;
#endif
}

uint32_t omg_thread_get_id(OMG_Omega* this, OMG_Thread* thread) {
#if OMG_IS_WIN && OMG_SUPPORT_THREADING
    if (OMG_ISNULL(thread))
        return (uint32_t)d_k32->GetCurrentThreadId();
    return ((OMG_ThreadWin*)thread)->id;
#else
    OMG_UNUSED(this, thread);
    return 0;
#endif
}

bool omg_thread_set_priority(OMG_Omega* this, OMG_Thread* thread, int priority) {
#if OMG_IS_WIN && OMG_SUPPORT_THREADING
    int prior;
    if (priority == OMG_THREAD_PRIORITY_LOW)
        prior = THREAD_PRIORITY_LOWEST;
    else if (priority == OMG_THREAD_PRIORITY_HIGH)
        prior = THREAD_PRIORITY_HIGHEST;
    else if (priority == OMG_THREAD_PRIORITY_TIME_CRITICAL)
        prior = THREAD_PRIORITY_TIME_CRITICAL;
    else
        prior = THREAD_PRIORITY_NORMAL;
    if (!d_k32->SetThreadPriority(OMG_ISNULL(thread) ? d_k32->GetCurrentThread() : ((OMG_ThreadWin*)thread)->handle, prior)) {
        return true;
    }
    return false;
#else
    OMG_UNUSED(this, thread, priority);
    return true;
#endif
}

bool omg_thread_wait(OMG_Omega* this, OMG_Thread* thread, int* status) {
#if OMG_IS_WIN && OMG_SUPPORT_THREADING
    if (OMG_ISNULL(thread)) {
        if (OMG_ISNOTNULL(status))
            *status = -1;
        return true;
    }
    HANDLE handle = ((OMG_ThreadWin*)thread)->handle;
    d_k32->WaitForSingleObjectEx(handle, INFINITE, FALSE);
    if (OMG_ISNOTNULL(status)) {
        *status = ((OMG_ThreadWin*)thread)->status;
    }
    OMG_FREE(this->mem, thread);
    d_k32->CloseHandle(handle);
    return false;
#else
    OMG_UNUSED(this, thread, status);
    return true;
#endif
}

bool omg_thread_detach(OMG_Omega* this, OMG_Thread* thread) {
#if OMG_IS_WIN && OMG_SUPPORT_THREADING
    if (OMG_ISNULL(thread))
        return false;
    OMG_ThreadWin* thread_win = (OMG_ThreadWin*)thread;
    HANDLE handle = thread_win->handle;
    if (thread_win->running) {
        thread_win->should_free = true;
    }
    else {
        OMG_FREE(this->mem, thread);
    }
    bool res = !d_k32->CloseHandle(handle);
    return res;
#else
    OMG_UNUSED(this, thread);
    return true;
#endif
}

bool omg_omg_init(OMG_Omega* this) {
    this->type = OMG_OMEGA_TYPE_NONE;
#if OMG_IS_WIN
    this->should_free_dwm = this->should_free_g32 = this->should_free_k32 = this->should_free_ntdll = this->should_free_u32 = this->should_free_uxtheme = this->should_free_std = this->should_free_msvcrt = false;
#endif
#if OMG_SUPPORT_LIBC
    this->should_free_libc = false;
#endif
    this->extra1 = this->extra2 = this->extra3 = this->extra4 = this->extra5 = NULL;
    this->std = NULL;
    this->clock = NULL;
    this->keyboard_state = NULL;
    this->keymap = NULL;
    this->looping = false;
    this->emulate_mouse = true;
    this->enable_paint = true;
    this->supports_screen_keyboard = false;
    this->should_free_mem = false;
    this->audio_type = OMG_AUDIO_TYPE_AUTO;
    this->theme = OMG_THEME_NONE;
    this->win_build_number = this->win_major_ver = this->win_minor_ver = 0;
    this->scale.x = this->scale.y = 1.0f;
    this->mouse_state = 0;
    if (this->log_level == -1)
        this->log_level = OMG_LOG_MIN_LEVEL;
    if (this->log_level_omg == -1)
        this->log_level_omg = OMG_LOG_OMG_MIN_LEVEL;
    if (OMG_ISNULL(this->log_str_type))
        this->log_str_type = omg_log_str_type;
    this->log_set_level = omg_log_set_level;
    this->destroy = omg_destroy;
    this->app_init = omg_app_init;
    this->app_quit = omg_app_quit;
    this->auto_loop_run = omg_auto_loop_run;
    this->auto_loop_stop = omg_auto_loop_stop;
    this->set_text_input_state = omg_set_text_input_state;
    this->delay = omg_delay;
    this->reset_event_handlers = omg_reset_event_handlers;
    this->winmgr_alloc = omg_alloc_winmgr;
    this->winmgr_free = omg_free_winmgr;
    this->audio_alloc = omg_audio_alloc;
    this->audio_free = omg_audio_free;
    this->fs_is_file_or_dir = omg_fs_is_file_or_dir;
    this->fs_remove_file_or_dir = omg_fs_remove_file_or_dir;
    this->fs_move = omg_fs_move;
    this->env_get = omg_env_get;
    this->env_set = omg_env_set;
    this->message_box = omg_message_box;
    this->thread_create = omg_thread_create;
    this->thread_get_id = omg_thread_get_id;
    this->thread_set_priority = omg_thread_set_priority;
    this->thread_detach = omg_thread_detach;
    this->thread_wait = omg_thread_wait;
    OMG_BEGIN_POINTER_CAST();
#if OMG_HAS_STD
    this->file_from_fp = omg_file_std_from_path;
    this->sz_file = sizeof(OMG_FileStd);
#else
    this->file_from_fp = omg_file_from_fp;
    this->sz_file = sizeof(OMG_File);
#endif
    this->file_from_mem = omg_file_from_mem;
    OMG_END_POINTER_CAST();
    omg_reset_event_handlers(this);
    omg_def_omega = this;
    return false;
}
