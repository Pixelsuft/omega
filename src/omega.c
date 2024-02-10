#include <omega/omega.h>
#if OMG_HAS_STD
#include <stdio.h>
#endif
#include <omega/audio_fmod.h>
#include <omega/audio_sdl2.h>
#define file_base ((OMG_File*)file)
#define file_omg ((OMG_Omega*)file_base->omg)

static OMG_Omega* omg_def_omega = NULL;

OMG_Omega* omg_get_default_omega(void) {
    return omg_def_omega;
}

OMG_Omega* omg_create(OMG_EntryData* data) {
    OMG_UNUSED(data);
    return NULL;
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
    this->log_info_str = NULL;
    this->log_warn_str = NULL;
    this->log_error_str = NULL;
    this->log_fatal_str = NULL;
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

bool omg_log_info_str(OMG_Omega* this, const OMG_String* data) {
    OMG_UNUSED(this, data);
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
// TODO: support 64-bit types
bool omg_file_std_destroy(OMG_FileStd* file) {
    bool res = false;
    FILE* handle = (FILE*)file->file;
    file->file = NULL;
    OMG_BEGIN_POINTER_CAST();
    omg_file_destroy(file);
    OMG_END_POINTER_CAST();
    if (OMG_ISNULL(handle)) {
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

bool omg_omg_init(OMG_Omega* this) {
    this->type = OMG_OMEGA_TYPE_NONE;
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
    this->scale.x = this->scale.y = 1.0f;
    this->mouse_state = 0;
    if (this->log_level == -1)
        this->log_level = OMG_LOG_MIN_LEVEL;
    if (this->log_level_omg == -1)
        this->log_level_omg = OMG_LOG_OMG_MIN_LEVEL;
    if (OMG_ISNULL(this->log_info_str))
        this->log_info_str = omg_log_info_str;
    if (OMG_ISNULL(this->log_warn_str))
        this->log_warn_str = omg_log_info_str;
    if (OMG_ISNULL(this->log_error_str))
        this->log_error_str = omg_log_info_str;
    if (OMG_ISNULL(this->log_fatal_str))
        this->log_fatal_str = omg_log_info_str;
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
