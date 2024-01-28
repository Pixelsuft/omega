#include <omega/audio_fmod.h>

#if OMG_SUPPORT_FMOD
#include <omega/omega.h>
#define base ((OMG_Audio*)this)
#define mus_base ((OMG_Music*)mus)
#define snd_base ((OMG_Sound*)snd)
#define omg_base ((OMG_Omega*)base->omg)
#define HAS_ERROR(res) ((res) != FMOD_OK)
#define IS_PLAYING(data) (OMG_ISNOTNULL((data)->channel))

#define _FMOD_AUDIO_TYPE(audio_type, format) do { \
    if (format & OMG_AUDIO_FORMAT_WAV) \
        audio_type = FMOD_SOUND_TYPE_WAV; \
    else if (format & OMG_AUDIO_FORMAT_FLAC) \
        audio_type = FMOD_SOUND_TYPE_FLAC; \
    else if (format & OMG_AUDIO_FORMAT_MOD) \
        audio_type = FMOD_SOUND_TYPE_MOD; \
    else if (format & OMG_AUDIO_FORMAT_MP3) \
        audio_type = FMOD_SOUND_TYPE_MPEG; \
    else if (format & OMG_AUDIO_FORMAT_OGG) \
        audio_type = FMOD_SOUND_TYPE_OGGVORBIS; \
    else if (format & OMG_AUDIO_FORMAT_MID) \
        audio_type = FMOD_SOUND_TYPE_MIDI; \
    else if (format & OMG_AUDIO_FORMAT_OPUS) \
        audio_type = FMOD_SOUND_TYPE_OPUS; \
} while (0)

static FMOD_RESULT OMG_FMOD_STD_PREFIX (*FMOD_Channel_GetUserData_callback)(FMOD_CHANNEL*, void**) = NULL;

bool omg_audio_fmod_update(OMG_AudioFmod* this) {
    int res = this->fmod.FMOD_System_Update(this->sys);
    if (HAS_ERROR(res)) {
        _OMG_LOG_WARN(omg_base, "Failed to update FMOD system (", FMOD_ErrorString(res), ")");
        return true;
    }
    return false;
}

bool omg_audio_fmod_destroy(OMG_AudioFmod* this) {
    if (!base->inited)
        return false;
    int retval = false;
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_System_Close(this->sys))) {
        _OMG_LOG_WARN(omg_base, "Failed to close FMOD system (", FMOD_ErrorString(res), ")");
        retval = true;
    }
    if (HAS_ERROR(res = this->fmod.FMOD_System_Release(this->sys))) {
        _OMG_LOG_WARN(omg_base, "Failed to release FMOD system (", FMOD_ErrorString(res), ")");
        retval = true;
    }
    retval = omg_fmod_dll_free(&this->fmod) || retval;
    return retval;
}

bool omg_audio_fmod_mus_set_volume(OMG_AudioFmod* this, OMG_MusicFmod* mus, float volume) {
    mus->vol_cache = volume;
    if (IS_PLAYING(mus)) {
        int res;
        if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetVolume(mus->channel, volume))) {
            _OMG_LOG_WARN(omg_base, "Failed to set audio volume (", FMOD_ErrorString(res), ")");
            return true;
        }
    }
    return false;
}

FMOD_RESULT omg_audio_fmod_mus_callback(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2) {
    OMG_UNUSED(commanddata1, commanddata2);
    if ((callbacktype == FMOD_CHANNELCONTROL_CALLBACK_END) && (controltype == FMOD_CHANNELCONTROL_CHANNEL)) {
        if (OMG_ISNULL(FMOD_Channel_GetUserData_callback)) {
            return FMOD_ERR_INVALID_HANDLE;
        }
        int res;
        OMG_MusicFmod* mus = NULL;
        if (HAS_ERROR(res = FMOD_Channel_GetUserData_callback((FMOD_CHANNEL*)channelcontrol, (void**)&mus))) {
            return res;
        }
        if (OMG_ISNULL(mus)) {
            return FMOD_ERR_INVALID_HANDLE;
        }
        mus->channel = NULL;
    }
    return FMOD_OK;
}

bool omg_audio_fmod_mus_stop(OMG_AudioFmod* this, OMG_MusicFmod* mus) {
    if (IS_PLAYING(mus)) {
        int res;
        if (HAS_ERROR(res = this->fmod.FMOD_Channel_Stop(mus->channel))) {
            _OMG_LOG_ERROR(omg_base, "Failed to stop audio (", FMOD_ErrorString(res), ")");
            return true;
        }
    }
    return false;
}

bool omg_audio_fmod_mus_play(OMG_AudioFmod* this, OMG_MusicFmod* mus, int loops, double pos, double fade_in) {
    OMG_UNUSED(fade_in); // TODO
    if (OMG_ISNOTNULL(mus->channel))
        omg_audio_fmod_mus_stop(this, mus);
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_Sound_SetLoopCount(mus->mus, loops))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio loop count (", FMOD_ErrorString(res), ")");
    }
    if (HAS_ERROR(res = this->fmod.FMOD_System_PlaySound(this->sys, mus->mus, NULL, 1, &mus->channel))) {
        mus->channel = NULL;
        _OMG_LOG_WARN(omg_base, "Failed to play audio (", FMOD_ErrorString(res), ")");
        return true;
    }
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetPosition(mus->channel, (int)(pos * 1000.0), FMOD_TIMEUNIT_MS))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio start pos (", FMOD_ErrorString(res), ")");
    }
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetUserData(mus->channel, (void*)mus))) {
        _OMG_LOG_WARN(omg_base, "Failed to pass audio handle to channel data (", FMOD_ErrorString(res), ")");
    }
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetVolume(mus->channel, mus->vol_cache))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio volume (", FMOD_ErrorString(res), ")");
    }
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetCallback(mus->channel, (FMOD_CHANNELCONTROL_CALLBACK)omg_audio_fmod_mus_callback))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio channel end callback (", FMOD_ErrorString(res), ")");
    }
    if ((mus->pitch_cache != 1.0f) && HAS_ERROR(res = this->fmod.FMOD_Channel_SetPitch(mus->channel, mus->pitch_cache))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio pitch (", FMOD_ErrorString(res), ")");
    }
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetPaused(mus->channel, 0))) {
        _OMG_LOG_WARN(omg_base, "Failed to resume audio (", FMOD_ErrorString(res), ")");
    }
    return false;
}

bool omg_audio_fmod_mus_destroy(OMG_AudioFmod* this, OMG_MusicFmod* mus) {
    if (OMG_ISNULL(mus) || OMG_ISNULL(mus->mus))
        return false;
    mus->channel = NULL;
    bool retval = false;
    int res;
    omg_audio_fmod_mus_stop(this, mus);
    if (HAS_ERROR(res = this->fmod.FMOD_Sound_Release(mus->mus))) {
        _OMG_LOG_ERROR(omg_base, "Failed to release audio (", FMOD_ErrorString(res), ")");
        retval = true;
    }
    if (OMG_ISNOTNULL(mus->temp_buf)) {
        OMG_FREE(omg_base->mem, mus->temp_buf);
        mus->temp_buf = NULL;
    }
    omg_audio_mus_destroy(base, mus_base);
    return retval;
}

double omg_audio_fmod_mus_get_pos(OMG_AudioFmod* this, OMG_MusicFmod* mus) {
    if (!IS_PLAYING(mus))
        return 0.0;
    int res;
    unsigned int pos;
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_GetPosition(mus->channel, &pos, FMOD_TIMEUNIT_MS))) {
        _OMG_LOG_WARN(omg_base, "Failed to get audio position (", FMOD_ErrorString(res), ")");
        return -1.0;
    }
    return (double)pos / 1000.0;
}

bool omg_audio_fmod_mus_set_pos(OMG_AudioFmod* this, OMG_MusicFmod* mus, double pos) {
    if (pos < 0.0)
        pos = 0.0;
    else if (pos > mus_base->duration) // Should I use this hack???
        pos = mus_base->duration - 0.001;
    if (!IS_PLAYING(mus))
        return false;
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetPosition(mus->channel, (unsigned int)(pos * 1000.0), FMOD_TIMEUNIT_MS))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio position (", FMOD_ErrorString(res), ")");
        return true;
    }
    return false;
}

bool omg_audio_fmod_mus_set_speed(OMG_AudioFmod* this, OMG_MusicFmod* mus, float speed) {
    mus->pitch_cache = speed;
    if (!IS_PLAYING(mus))
        return false;
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetPitch(mus->channel, speed))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio pitch (", FMOD_ErrorString(res), ")");
        return true;
    }
    return false;
}

OMG_MusicFmod* omg_audio_fmod_mus_from_fp(OMG_AudioFmod* this, OMG_MusicFmod* mus, const OMG_String* path, int format) {
    OMG_UNUSED(format);
    if (omg_string_ensure_null((OMG_String*)path))
        return NULL;
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicFmod));
        if (OMG_ISNULL(mus))
            return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_System_CreateStream(
        this->sys,
        path->ptr,
        FMOD_LOOP_NORMAL | FMOD_2D,
        NULL,
        &mus->mus
    ))) {
        _OMG_LOG_ERROR(omg_base, "Failed to load music ", path->ptr, " (", FMOD_ErrorString(res), ")");
        omg_audio_mus_destroy(base, mus_base);
        return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    mus->temp_buf = NULL;
    mus->vol_cache = 1.0f;
    mus->pitch_cache = 1.0f;
    mus->channel = NULL;
    unsigned int len_buf;
    if (HAS_ERROR(res = this->fmod.FMOD_Sound_GetLength(mus->mus, &len_buf, FMOD_TIMEUNIT_MS))) {
        mus_base->duration = -1.0;
        _OMG_LOG_ERROR(omg_base, "Failed to get music length ", path->ptr, " (", FMOD_ErrorString(res), ")");
    }
    else
        mus_base->duration = (double)len_buf / 1000.0;
    return mus;
}

FMOD_RESULT omg_audio_fmod_open_callback(const char* name, unsigned int* file_size, void** handle, void* userdata) {
    OMG_UNUSED(userdata);
    *handle = (void*)name;
    OMG_File* file = (OMG_File*)name;
    int64_t file_size_tmp = file->get_size(file);
    if (file_size_tmp <= 0)
        return FMOD_ERR_FILE_BAD;
    *file_size = (unsigned int)file_size_tmp;
    return FMOD_OK;
}

FMOD_RESULT omg_audio_fmod_close_callback(void* handle, void* userdata) {
    OMG_UNUSED(userdata);
    ((OMG_File*)handle)->destroy((OMG_File*)handle);
    return FMOD_OK;
}

FMOD_RESULT omg_audio_fmod_seek_callback(void* handle, unsigned int pos, void* userdata) {
    OMG_UNUSED(userdata);
    OMG_File* file = (OMG_File*)handle;
    if (file->seek(file, (int64_t)pos, OMG_FILE_SEEK_SET) < 0)
        return FMOD_ERR_FILE_COULDNOTSEEK;
    return FMOD_OK;
}

FMOD_RESULT omg_audio_fmod_read_callback(void* handle, void* buffer, unsigned int sizebytes, unsigned int* bytesread, void* userdata) {
    OMG_UNUSED(userdata);
    OMG_File* file = (OMG_File*)handle;
    if (file->get_size(file) == file->tell(file)) {
        *bytesread = 0;
        return FMOD_ERR_FILE_EOF;
    }
    size_t res = file->read(file, buffer, 1, (size_t)sizebytes);
    if (res == 0) {
        *bytesread = 0;
        return FMOD_ERR_FILE_BAD;
    }
    *bytesread = (unsigned int)res;
    return FMOD_OK;
}

OMG_MusicFmod* omg_audio_fmod_mus_from_file(OMG_AudioFmod* this, OMG_MusicFmod* mus, OMG_File* file, bool destroy_file, int format) {
    // TODO: auto close file
    OMG_UNUSED(destroy_file);
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicFmod));
        if (OMG_ISNULL(mus))
            return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    FMOD_CREATESOUNDEXINFO info;
    omg_base->std->memset(&info, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    info.fileuserdata = this;
    info.fileuseropen = omg_audio_fmod_open_callback;
    info.fileuserclose = omg_audio_fmod_close_callback;
    info.fileuserread = omg_audio_fmod_read_callback;
    info.fileuserseek = omg_audio_fmod_seek_callback;
    _FMOD_AUDIO_TYPE(info.suggestedsoundtype, format);
    mus->temp_buf = NULL;
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_System_CreateStream(
        this->sys,
        (const char*)file,
        FMOD_LOOP_NORMAL | FMOD_2D,
        &info,
        &mus->mus
    ))) {
        _OMG_LOG_ERROR(omg_base, "Failed to load music from file handle (", FMOD_ErrorString(res), ")");
        omg_audio_mus_destroy(base, mus_base);
        return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    mus->pitch_cache = 1.0f;
    mus->vol_cache = 1.0f;
    mus->channel = NULL;
    unsigned int len_buf;
    if (HAS_ERROR(res = this->fmod.FMOD_Sound_GetLength(mus->mus, &len_buf, FMOD_TIMEUNIT_MS))) {
        mus_base->duration = -1.0;
        _OMG_LOG_ERROR(omg_base, "Failed to get music length (", FMOD_ErrorString(res), ")");
    }
    else
        mus_base->duration = (double)len_buf / 1000.0;
    _OMG_LOG_INFO(omg_base, mus_base->duration);
    return mus;
}

OMG_MusicFmod* omg_audio_fmod_mus_from_mem(OMG_AudioFmod* this, OMG_MusicFmod* mus, const void* data, size_t size, int format) {
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicFmod));
        if (OMG_ISNULL(mus))
            return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    FMOD_CREATESOUNDEXINFO info;
    omg_base->std->memset(&info, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    info.length = (unsigned int)size;
    _FMOD_AUDIO_TYPE(info.suggestedsoundtype, format);
    if (format & OMG_AUDIO_FORMAT_INTERNAL)
        mus->temp_buf = (void*)data;
    else
        mus->temp_buf = NULL;
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_System_CreateStream(
        this->sys,
        (const char*)data,
        FMOD_LOOP_NORMAL | FMOD_2D | FMOD_OPENMEMORY,
        &info,
        &mus->mus
    ))) {
        _OMG_LOG_ERROR(omg_base, "Failed to load music from memory (", FMOD_ErrorString(res), ")");
        omg_audio_mus_destroy(base, mus_base);
        return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    mus->pitch_cache = 1.0f;
    mus->vol_cache = 1.0f;
    mus->channel = NULL;
    unsigned int len_buf;
    if (HAS_ERROR(res = this->fmod.FMOD_Sound_GetLength(mus->mus, &len_buf, FMOD_TIMEUNIT_MS))) {
        mus_base->duration = -1.0;
        _OMG_LOG_ERROR(omg_base, "Failed to get music length (", FMOD_ErrorString(res), ")");
    }
    else
        mus_base->duration = (double)len_buf / 1000.0;
    _OMG_LOG_INFO(omg_base, mus_base->duration);
    return mus;
}

OMG_MusicFmod* omg_audio_fmod_snd_from_fp(OMG_AudioFmod* this, OMG_MusicFmod* mus, const OMG_String* path, int format) {
    OMG_UNUSED(format);
    if (omg_string_ensure_null((OMG_String*)path))
        return NULL;
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicFmod));
        if (OMG_ISNULL(mus))
            return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_System_CreateSound(
        this->sys,
        path->ptr,
        FMOD_LOOP_NORMAL | FMOD_2D,
        NULL,
        &mus->mus
    ))) {
        _OMG_LOG_ERROR(omg_base, "Failed to load sound ", path->ptr, " (", FMOD_ErrorString(res), ")");
        omg_audio_mus_destroy(base, mus_base);
        return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    mus->temp_buf = NULL;
    mus->pitch_cache = 1.0f;
    mus->vol_cache = 1.0f;
    mus->channel = NULL;
    unsigned int len_buf;
    if (HAS_ERROR(res = this->fmod.FMOD_Sound_GetLength(mus->mus, &len_buf, FMOD_TIMEUNIT_MS))) {
        mus_base->duration = -1.0;
        _OMG_LOG_ERROR(omg_base, "Failed to get sound length ", path->ptr, " (", FMOD_ErrorString(res), ")");
    }
    else
        mus_base->duration = (double)len_buf / 1000.0;
    return mus;
}

OMG_MusicFmod* omg_audio_fmod_snd_from_mem(OMG_AudioFmod* this, OMG_MusicFmod* mus, const void* data, size_t size, int format) {
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicFmod));
        if (OMG_ISNULL(mus))
            return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    FMOD_CREATESOUNDEXINFO info;
    omg_base->std->memset(&info, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    info.length = (unsigned int)size;
    _FMOD_AUDIO_TYPE(info.suggestedsoundtype, format);    
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_System_CreateSound(
        this->sys,
        (const char*)data,
        FMOD_LOOP_NORMAL | FMOD_2D | FMOD_OPENMEMORY,
        &info,
        &mus->mus
    ))) {
        _OMG_LOG_ERROR(omg_base, "Failed to load sound from memory (", FMOD_ErrorString(res), ")");
        omg_audio_mus_destroy(base, mus_base);
        return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    mus->temp_buf = NULL;
    mus->pitch_cache = 1.0f;
    mus->vol_cache = 1.0f;
    mus->channel = NULL;
    unsigned int len_buf;
    if (HAS_ERROR(res = this->fmod.FMOD_Sound_GetLength(mus->mus, &len_buf, FMOD_TIMEUNIT_MS))) {
        mus_base->duration = -1.0;
        _OMG_LOG_ERROR(omg_base, "Failed to get sound length (", FMOD_ErrorString(res), ")");
    }
    else
        mus_base->duration = (double)len_buf / 1000.0;
    _OMG_LOG_INFO(omg_base, mus_base->duration);
    return mus;
}

bool omg_audio_fmod_init(OMG_AudioFmod* this) {
    omg_audio_init((OMG_Audio*)this);
    if (omg_fmod_dll_load(&this->fmod, omg_base->fmod_dll_path)) {
        _OMG_LOG_ERROR(omg_base, "Failed to load FMOD dll");
        return true;
    }
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_System_Create(&this->sys, FMOD_VERSION))) {
        omg_fmod_dll_free(&this->fmod);
        return true;
    }
    if (HAS_ERROR(res = this->fmod.FMOD_System_Init(
        this->sys,
        OMG_MAX_PLAYING_MUSIC + OMG_MAX_PLAYING_SOUND,
        FMOD_INIT_NORMAL,
        NULL
    ))) {
        _OMG_LOG_WARN(omg_base, "Failed to init FMOD system (", FMOD_ErrorString(res), ")");
        this->fmod.FMOD_System_Close(this->sys);
        omg_fmod_dll_free(&this->fmod);
        return true;
    }
    FMOD_Channel_GetUserData_callback = this->fmod.FMOD_Channel_GetUserData;
    OMG_BEGIN_POINTER_CAST();
    base->update = omg_audio_fmod_update;
    base->destroy = omg_audio_fmod_destroy;
    base->mus_from_fp = omg_audio_fmod_mus_from_fp;
    base->mus_from_mem = omg_audio_fmod_mus_from_mem;
    base->mus_from_file = omg_audio_fmod_mus_from_file;
    base->mus_destroy = omg_audio_fmod_mus_destroy;
    base->mus_set_volume = omg_audio_fmod_mus_set_volume;
    base->mus_play = omg_audio_fmod_mus_play;
    base->mus_stop = omg_audio_fmod_mus_stop;
    base->mus_get_pos = omg_audio_fmod_mus_get_pos;
    base->mus_set_pos = omg_audio_fmod_mus_set_pos;
    base->mus_set_speed = omg_audio_fmod_mus_set_speed;
    base->snd_from_fp = omg_audio_fmod_snd_from_fp;
    base->snd_from_mem = omg_audio_fmod_snd_from_mem;
    // Hacky
    base->snd_destroy = omg_audio_fmod_mus_destroy;
    base->snd_set_volume = omg_audio_fmod_mus_set_volume;
    base->snd_play = omg_audio_fmod_mus_play;
    base->snd_stop = omg_audio_fmod_mus_stop;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
