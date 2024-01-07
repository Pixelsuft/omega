#include <omega/omega.h>
#if OMG_HAS_STD
#include <stdio.h>
#endif
#define file_base ((OMG_File*)file)
#define file_omg ((OMG_Omega*)file_base->omg)

static OMG_Omega* omg_def_omega = NULL;

OMG_API OMG_Omega* omg_get_default_omega(void) {
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
    static uint8_t buf[1024 * 3];
    return (OMG_Omega*)buf;
}

void omg_free_omega_internal(OMG_Omega* this) {
    OMG_UNUSED(this);
}

void omg_fill_on_create(OMG_Omega* this, OMG_EntryData* data) {
    this->entry_data = data;
    this->mem = NULL;
    this->winmgr = NULL;
    this->log_level = this->log_level_omg = this->log_level_lib = -1;
    this->sdl2_dll_path = NULL;
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
    this->winmgr->init = omg_winmgr_init;
    return false;
}

bool omg_free_winmgr(OMG_Omega* this) {
    if (OMG_ISNULL(this->winmgr))
        return false;
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
}

bool omg_file_destroy(OMG_File* file) {
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

OMG_File* omg_file_from_path(OMG_Omega* this, OMG_File* file, const OMG_String* path, int mode) {
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
    file->destroy = omg_file_destroy;
    file->get_size = omg_file_get_size;
    file->seek = omg_file_seek;
    file->tell = omg_file_tell;
    file->read = omg_file_read;
    file->write = omg_file_write;
    return file;
}

#if OMG_HAS_STD
// TODO: support 64-bit types
bool omg_file_std_destroy(OMG_FileStd* file) {
    bool res = false;
    if (OMG_ISNULL(file->file)) {
        if (fclose(file->file) != 0) {
            _OMG_LOG_WARN(file_omg, "Failed to std close file ", file_base->fp.ptr);
        }
        file->file = NULL;
    }
    OMG_BEGIN_POINTER_CAST();
    omg_file_destroy(file);
    OMG_END_POINTER_CAST();
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
    if ((res < (size * maxnum)) && !feof(file->file)) {
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
    file = omg_file_from_path(this, file, path, mode);
    if (OMG_ISNULL(file))
        return NULL;
    file->file = fopen(path->ptr, str_mode);
    if (OMG_ISNULL(file->file)) {
        _OMG_LOG_ERROR(this, "Failed to std open file ", path->ptr);
        omg_file_destroy(file);
        return NULL;
    }
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
    this->sz_file = sizeof(OMG_File);
    this->theme = OMG_THEME_NONE;
    this->scale.x = this->scale.y = 1.0f;
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
#if OMG_HAS_STD
    OMG_BEGIN_POINTER_CAST();
    this->file_from_path = omg_file_std_from_path;
    OMG_END_POINTER_CAST();
#else
    this->file_from_path = omg_file_from_path;
#endif
    omg_reset_event_handlers(this);
    omg_def_omega = this;
    return false;
}
