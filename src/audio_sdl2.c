#include <omega/audio_sdl2.h>

#if OMG_SUPPORT_SDL2_MIXER
#include <omega/omega.h>
#define base ((OMG_Audio*)this)
#define mus_base ((OMG_Music*)mus)
#define snd_base ((OMG_Sound*)snd)
#define omg_base ((OMG_Omega*)base->omg)
#define MIX_GETERROR() (OMG_ISNULL(this->sdl2) ? "" : this->sdl2->SDL_GetError())
#define MUS_IS_PLAYING() (OMG_ISNOTNULL(cur_mus_cache) && (cur_mus_cache->mus == mus->mus) && this->mix.Mix_PlayingMusic())
#define SND_IS_PLAYING() (snd->channel >= 0)
#define _MUS_TYPE_FORMAT(mus_type, format) do { \
    if (format & OMG_AUDIO_FORMAT_WAV) \
        mus_type = MUS_WAV; \
    else if (format & OMG_AUDIO_FORMAT_FLAC) \
        mus_type = MUS_FLAC; \
    else if (format & OMG_AUDIO_FORMAT_MOD) \
        mus_type = MUS_MOD; \
    else if (format & OMG_AUDIO_FORMAT_MP3) \
        mus_type = MUS_MP3; \
    else if (format & OMG_AUDIO_FORMAT_OGG) \
        mus_type = MUS_OGG; \
    else if (format & OMG_AUDIO_FORMAT_MID) \
        mus_type = MUS_MID; \
    else if (format & OMG_AUDIO_FORMAT_OPUS) \
        mus_type = MUS_OPUS; \
    else \
        mus_type = MUS_NONE; \
} while (0)

static OMG_AudioSdl2* cur_audio_cache = NULL;
static OMG_MusicSdl2* cur_mus_cache = NULL;

bool omg_audio_sdl2_destroy(OMG_AudioSdl2* this) {
    if (!base->inited)
        return false;
    bool res = false;
    this->mix.Mix_CloseAudio();
    this->mix.Mix_Quit();
    if (OMG_ISNOTNULL(this->sdl2)) {
        this->sdl2->SDL_AudioQuit();
    }
    res = omg_sdl2_mixer_dll_free(&this->mix) || res;
    cur_mus_cache = NULL;
    cur_audio_cache = NULL;
    return res;
}

bool omg_audio_sdl2_mus_destroy(OMG_AudioSdl2* this, OMG_MusicSdl2* mus) {
    if (OMG_ISNULL(mus) || OMG_ISNULL(mus->mus))
        return false;
    this->mix.Mix_FreeMusic(mus->mus);
    mus->mus = NULL;
    omg_audio_mus_destroy(base, mus_base);
    return false;
}

OMG_MusicSdl2* omg_audio_sdl2_mus_from_fp(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, const OMG_String* path, int format) {
    OMG_UNUSED(format);
    if (omg_string_ensure_null((OMG_String*)path))
        return NULL;
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicSdl2));
        if (OMG_ISNULL(mus))
            return (OMG_MusicSdl2*)omg_audio_dummy_mus_alloc(base, mus_base);
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    mus->mus = this->mix.Mix_LoadMUS(path->ptr);
    if (OMG_ISNULL(mus->mus)) {
        omg_audio_mus_destroy(base, mus_base);
        _OMG_LOG_ERROR(omg_base, "Failed to open music ", path->ptr, " (", MIX_GETERROR(), ")");
        return (OMG_MusicSdl2*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    if (OMG_ISNULL(this->mix.Mix_MusicDuration))
        mus_base->duration = -1.0;
    else
        mus_base->duration = this->mix.Mix_MusicDuration(mus->mus);
    mus_base->is_dummy = false;
    mus->vol_cache = MIX_MAX_VOLUME;
    mus->time_cache1 = 0;
    mus->time_cache2 = 0;
    return mus;
}

OMG_MusicSdl2* omg_audio_sdl2_mus_from_mem(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, const void* data, size_t size, int format) {
    if (OMG_ISNULL(this->sdl2))
        return NULL;
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicSdl2));
        if (OMG_ISNULL(mus))
            return (OMG_MusicSdl2*)omg_audio_dummy_mus_alloc(base, mus_base);
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    Mix_MusicType mus_type;
    _MUS_TYPE_FORMAT(mus_type, format);
    SDL_RWops* rw = this->sdl2->SDL_RWFromConstMem(data, (int)size);
    if (OMG_ISNULL(rw)) {
        _OMG_LOG_ERROR(omg_base, "Failed create RWops for SDL2_mixer music (", MIX_GETERROR(), ")");
        return (OMG_MusicSdl2*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    mus->mus = this->mix.Mix_LoadMUSType_RW(rw, mus_type, 1);
    if (OMG_ISNULL(mus->mus)) {
        omg_audio_mus_destroy(base, mus_base);
        _OMG_LOG_ERROR(omg_base, "Failed to open music from mem (", MIX_GETERROR(), ")");
        return (OMG_MusicSdl2*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    if (OMG_ISNULL(this->mix.Mix_MusicDuration))
        mus_base->duration = -1.0;
    else
        mus_base->duration = this->mix.Mix_MusicDuration(mus->mus);
    mus_base->is_dummy = false;
    mus->vol_cache = MIX_MAX_VOLUME;
    mus->time_cache1 = 0;
    mus->time_cache2 = 0;
    return mus;
}

int omg_sdl_rw_emu_close(SDL_RWops* rw) {
    OMG_File* file = (OMG_File*)((size_t)rw->write);
    return (int)file->destroy(file);
}

int64_t omg_sdl_rw_emu_size(SDL_RWops* rw) {
    OMG_File* file = (OMG_File*)((size_t)rw->write);
    return file->get_size(file);
}

int64_t omg_sdl_rw_emu_seek(SDL_RWops* rw, int64_t offset, int whence) {
    OMG_File* file = (OMG_File*)((size_t)rw->write);
    return file->seek(file, offset, whence);
}

size_t omg_sdl_rw_emu_read(SDL_RWops* rw, void* buf, size_t size, size_t maxnum) {
    OMG_File* file = (OMG_File*)((size_t)rw->write);
    if (file->tell(file) == file->get_size(file))
        return 0;
    return file->read(file, buf, size, maxnum);
}

OMG_MusicSdl2* omg_audio_sdl2_mus_from_file(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, OMG_File* file, bool destroy_file, int format) {
    if (OMG_ISNULL(this->sdl2))
        return NULL;
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicSdl2) + sizeof(SDL_RWops));
        if (OMG_ISNULL(mus))
            return (OMG_MusicSdl2*)omg_audio_dummy_mus_alloc(base, mus_base);
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    Mix_MusicType mus_type;
    _MUS_TYPE_FORMAT(mus_type, format);
    OMG_BEGIN_POINTER_CAST();
    SDL_RWops* rw = (SDL_RWops*)((size_t)mus + sizeof(OMG_MusicSdl2));
    rw->write = file;
    rw->type = SDL_RWOPS_MEMORY_RO;
    rw->close = omg_sdl_rw_emu_close;
    rw->size = omg_sdl_rw_emu_size;
    rw->seek = omg_sdl_rw_emu_seek;
    rw->read = omg_sdl_rw_emu_read;
    OMG_END_POINTER_CAST();
    if (OMG_ISNULL(rw)) {
        _OMG_LOG_ERROR(omg_base, "Failed create RWops for SDL2_mixer music (", MIX_GETERROR(), ")");
        return (OMG_MusicSdl2*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    mus->mus = this->mix.Mix_LoadMUSType_RW(rw, mus_type, destroy_file ? 1 : 0);
    if (OMG_ISNULL(mus->mus)) {
        omg_audio_mus_destroy(base, mus_base);
        _OMG_LOG_ERROR(omg_base, "Failed to open music from mem (", MIX_GETERROR(), ")");
        return (OMG_MusicSdl2*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    if (OMG_ISNULL(this->mix.Mix_MusicDuration))
        mus_base->duration = -1.0;
    else
        mus_base->duration = this->mix.Mix_MusicDuration(mus->mus);
    mus_base->is_dummy = false;
    mus->vol_cache = MIX_MAX_VOLUME;
    mus->time_cache1 = 0;
    mus->time_cache2 = 0;
    return mus;
}

bool omg_audio_sdl2_mus_play(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, int loops, double pos, double fade_in) {
    int res;
    mus->loops_cache = loops;
    if (!this->supports_get_pos)
        loops = 0;
    if ((pos == 0.0) && (fade_in == 0.0)) {
        res = this->mix.Mix_PlayMusic(mus->mus, loops);
    }
    else if (pos == 0.0) {
        res = this->mix.Mix_FadeInMusic(mus->mus, loops, (int)(fade_in * 1000.0));
    }
    else {
        res = this->mix.Mix_FadeInMusicPos(mus->mus, loops, (int)(fade_in * 1000.0), pos);
    }
    if (res < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to play music (", MIX_GETERROR(), ")");
        return true;
    }
    cur_mus_cache = mus;
    this->mix.Mix_VolumeMusic(mus->vol_cache);
    if (!this->supports_get_pos)
        mus->time_cache1 = this->sdl2->SDL_GetTicks64();
    return false;
}

bool omg_audio_sdl2_update(OMG_AudioSdl2* this) {
    if (this->supports_get_pos || OMG_ISNULL(cur_mus_cache))
        return false;
    if (!this->mix.Mix_PlayingMusic() && !this->mix.Mix_PausedMusic()) {
        uint64_t now = this->sdl2->SDL_GetTicks64();
        if (cur_mus_cache->loops_cache > 0)
            cur_mus_cache->loops_cache--;
        if (cur_mus_cache->parent.duration < 0.0)
            cur_mus_cache->parent.duration = (double)(now - cur_mus_cache->time_cache1) / 1000.0;
        if (cur_mus_cache->loops_cache != 0)
            omg_audio_sdl2_mus_play(this, cur_mus_cache, cur_mus_cache->loops_cache, 0.0, 0.0);
    }
    return false;
}

bool omg_audio_sdl2_mus_set_volume(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, float volume) {
    mus->vol_cache = (int)(volume * (float)MIX_MAX_VOLUME);
    if (MUS_IS_PLAYING())
        this->mix.Mix_VolumeMusic(mus->vol_cache);
    return false;
}

bool omg_audio_sdl2_mus_stop(OMG_AudioSdl2* this, OMG_MusicSdl2* mus) {
    if (MUS_IS_PLAYING()) {
        this->mix.Mix_HaltMusic();
        cur_mus_cache = NULL;
    }
    return false;
}

double omg_audio_sdl2_mus_get_pos(OMG_AudioSdl2* this, OMG_MusicSdl2* mus) {
    // Is this right???
    if (!MUS_IS_PLAYING())
        return 0.0;
    if (this->supports_get_pos)
        return this->mix.Mix_GetMusicPosition(mus->mus);
    return (double)(this->sdl2->SDL_GetTicks64() - mus->time_cache1) / 1000.0;
}

bool omg_audio_sdl2_snd_stop(OMG_AudioSdl2* this, OMG_SoundSdl2* snd) {
    if (SND_IS_PLAYING())
        this->mix.Mix_HaltChannel(snd->channel);
    return false;
}

bool omg_audio_sdl2_mus_set_pos(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, double pos) {
    if (pos < 0.0)
        pos = 0.0;
    if (!MUS_IS_PLAYING())
        return false;
    if (this->mix.Mix_SetMusicPosition((OMG_IS_EMSCRIPTEN && !this->supports_get_pos) ? (pos/* / 2.0*/) : pos) < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to set music position (not implemented)");
        return true;
    }
    if (!this->supports_get_pos) {
        mus->time_cache1 = this->sdl2->SDL_GetTicks64() - (uint64_t)(pos * 1000.0);
    }
    return false;
}

bool omg_audio_sdl2_snd_destroy(OMG_AudioSdl2* this, OMG_SoundSdl2* snd) {
    if (OMG_ISNULL(snd) || OMG_ISNULL(snd->chunk))
        return false;
    omg_audio_sdl2_snd_stop(this, snd);
    this->mix.Mix_FreeChunk(snd->chunk);
    snd->chunk = NULL;
    omg_audio_snd_destroy(base, snd_base);
    return false;
}

OMG_SoundSdl2* omg_audio_sdl2_snd_from_fp(OMG_AudioSdl2* this, OMG_SoundSdl2* snd, const OMG_String* path, int format) {
    OMG_UNUSED(format);
    if (omg_string_ensure_null((OMG_String*)path))
        return NULL;
    if (OMG_ISNULL(snd)) {
        snd = OMG_MALLOC(omg_base->mem, sizeof(OMG_SoundSdl2));
        if (OMG_ISNULL(snd))
            return (OMG_SoundSdl2*)omg_audio_dummy_snd_alloc(base, snd_base);
        snd_base->was_allocated = true;
    }
    else
        snd_base->was_allocated = false;
    if (OMG_ISNOTNULL(this->mix.Mix_LoadWAV))
        snd->chunk = this->mix.Mix_LoadWAV(path->ptr);
    else if (OMG_ISNOTNULL(this->sdl2))
        snd->chunk = this->mix.Mix_LoadWAV_RW(this->sdl2->SDL_RWFromFile(path->ptr, "rb"), 1);
    else
        snd->chunk = NULL;
    if (OMG_ISNULL(snd->chunk)) {
        omg_audio_snd_destroy(base, snd_base);
        _OMG_LOG_ERROR(omg_base, "Failed to open sound ", path->ptr, " (", MIX_GETERROR(), ")");
        return (OMG_SoundSdl2*)omg_audio_dummy_snd_alloc(base, snd_base);
    }
    snd_base->duration = -1.0;
    snd_base->is_dummy = false;
    snd->vol_cache = MIX_MAX_VOLUME;
    snd->channel = -2;
    snd->pan_cache[0] = snd->pan_cache[1] = 255;
    return snd;
}

OMG_SoundSdl2* omg_audio_sdl2_snd_from_mem(OMG_AudioSdl2* this, OMG_SoundSdl2* snd, const void* data, size_t size, int format) {
    OMG_UNUSED(format);
    if (OMG_ISNULL(this->sdl2))
        return NULL;
    if (OMG_ISNULL(snd)) {
        snd = OMG_MALLOC(omg_base->mem, sizeof(OMG_SoundSdl2));
        if (OMG_ISNULL(snd))
            return (OMG_SoundSdl2*)omg_audio_dummy_snd_alloc(base, snd_base);
        snd_base->was_allocated = true;
    }
    else
        snd_base->was_allocated = false;
    SDL_RWops* rw = this->sdl2->SDL_RWFromConstMem(data, (int)size);
    if (OMG_ISNULL(rw)) {
        _OMG_LOG_ERROR(omg_base, "Failed create RWops for SDL2_mixer sound (", MIX_GETERROR(), ")");
        return (OMG_SoundSdl2*)omg_audio_dummy_snd_alloc(base, snd_base);
    }
    snd->chunk = this->mix.Mix_LoadWAV_RW(rw, 1);
    if (OMG_ISNULL(snd->chunk)) {
        omg_audio_snd_destroy(base, snd_base);
        _OMG_LOG_ERROR(omg_base, "Failed to open sound from mem (", MIX_GETERROR(), ")");
        return (OMG_SoundSdl2*)omg_audio_dummy_snd_alloc(base, snd_base);
    }
    snd_base->duration = -1.0;
    snd_base->is_dummy = false;
    snd->vol_cache = MIX_MAX_VOLUME;
    snd->channel = -2;
    snd->pan_cache[0] = snd->pan_cache[1] = 255;
    return snd;
}

bool omg_audio_sdl2_snd_set_volume(OMG_AudioSdl2* this, OMG_SoundSdl2* snd, float volume) {
    snd->vol_cache = (int)(volume * (float)MIX_MAX_VOLUME);
    if (SND_IS_PLAYING())
        this->mix.Mix_Volume(snd->channel, snd->vol_cache);
    // this->mix.Mix_VolumeChunk(snd->chunk, (int)(volume * (float)MIX_MAX_VOLUME));
    return false;
}

void omg_audio_sdl2_channel_finish_cb(int channel) {
    if (OMG_ISNULL(cur_audio_cache) || (channel < 0))
        return;
    OMG_SoundSdl2* snd = cur_audio_cache->play_cache[channel];
    if (OMG_ISNULL(snd))
        return;
    snd->channel = -2;
}

bool omg_audio_sdl2_snd_play(OMG_AudioSdl2* this, OMG_SoundSdl2* snd, int loops, double pos, double fade_in) {
    OMG_UNUSED(pos);
    omg_audio_sdl2_snd_stop(this, snd);
    int channel = -1;
    if (fade_in > 0.0)
        snd->channel = this->mix.Mix_FadeInChannelTimed(channel, snd->chunk, loops, (int)(fade_in * 1000.0), -1);
    else
        snd->channel = this->mix.Mix_PlayChannelTimed(channel, snd->chunk, loops, -1);
    if (snd->channel < 0) {
        snd->channel = -2;
        _OMG_LOG_ERROR(omg_base, "Failed to play sound (", MIX_GETERROR(), ")");
        return true;
    }
    this->mix.Mix_Volume(snd->channel, snd->vol_cache);
    this->play_cache[snd->channel] = snd;
    this->mix.Mix_ChannelFinished(omg_audio_sdl2_channel_finish_cb);
    if ((snd->pan_cache[0] < 255) || (snd->pan_cache[1] < 255))
        this->mix.Mix_SetPanning(snd->channel, snd->pan_cache[0], snd->pan_cache[1]);
    return false;
}

bool omg_audio_sdl2_mus_pause(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, bool paused) {
    if (!MUS_IS_PLAYING())
        return false;
    (paused ? this->mix.Mix_PauseMusic : this->mix.Mix_ResumeMusic)();
    return false;
}

bool omg_audio_sdl2_snd_pause(OMG_AudioSdl2* this, OMG_SoundSdl2* snd, bool paused) {
    if (!SND_IS_PLAYING())
        return false;
    (paused ? this->mix.Mix_Pause : this->mix.Mix_Resume)(snd->channel);
    return false;
}

bool omg_audio_sdl2_snd_set_panning(OMG_AudioSdl2* this, OMG_SoundSdl2* snd, float left, float right) {
    snd->pan_cache[0] = (uint8_t)(left * 255.0f);
    snd->pan_cache[1] = (uint8_t)(right * 255.0f);
    if (!SND_IS_PLAYING())
        return false;
    if (this->mix.Mix_SetPanning(snd->channel, snd->pan_cache[0], snd->pan_cache[1]) == 0) {
        _OMG_LOG_WARN(omg_base, "Failed to set sound panning (", MIX_GETERROR(), ")");
        return true;
    }
    return false;
}

bool omg_audio_sdl2_init(OMG_AudioSdl2* this) {
    omg_audio_init(base);
    if (omg_sdl2_mixer_dll_load(&this->mix, omg_base->sdl2_mixer_dll_path)) {
        _OMG_LOG_ERROR(omg_base, "Failed to load SDL2_mixer dll");
        return true;
    }
    if (OMG_ISNOTNULL(this->sdl2)) {
        if (this->sdl2->SDL_AudioInit(NULL) < 0)
            _OMG_LOG_WARN(omg_base, "Failed to init SDL2 Audio (", MIX_GETERROR(), ")");
    }
    int mix_formats = 0;
    if (base->init_flags & OMG_AUDIO_FORMAT_FLAC)
        mix_formats |= MIX_INIT_FLAC;
    if (base->init_flags & OMG_AUDIO_FORMAT_MOD)
        mix_formats |= MIX_INIT_MOD;
    if (base->init_flags & OMG_AUDIO_FORMAT_MP3)
        mix_formats |= MIX_INIT_MP3;
    if (base->init_flags & OMG_AUDIO_FORMAT_OGG)
        mix_formats |= MIX_INIT_OGG;
    if (base->init_flags & OMG_AUDIO_FORMAT_MID)
        mix_formats |= MIX_INIT_MID;
    if (base->init_flags & OMG_AUDIO_FORMAT_OPUS)
        mix_formats |= MIX_INIT_OPUS;
    int res = this->mix.Mix_Init(mix_formats);
    if ((mix_formats != 0) && (res == 0)) {
        _OMG_LOG_ERROR(omg_base, "Failed to init SDL2_mixer (", MIX_GETERROR(), ")");
        omg_sdl2_mixer_dll_free(&this->mix);
        return true;
    }
    else if (res < mix_formats)
        _OMG_LOG_WARN(omg_base, "Failed to init some SDL2_mixer formats (", MIX_GETERROR(), ")");
    if (OMG_ISNOTNULL(this->sdl2) && OMG_ISNOTNULL(this->sdl2->SDL_GetDefaultAudioInfo)) {
        SDL_AudioSpec spec;
        if (this->sdl2->SDL_GetDefaultAudioInfo(NULL, &spec, 0) < 0) {
            _OMG_LOG_WARN(omg_base, "Failed to get default audio info (", MIX_GETERROR(), ")");
        }
        else {
            if (base->freq < 0)
                base->freq = spec.freq;
            if (base->channels < 0)
                base->channels = (int)spec.channels;
        }
    }
    if (base->chunk_size < 0)
        base->chunk_size = 2048;
    if (base->freq < 0)
        base->freq = MIX_DEFAULT_FREQUENCY;
    if (base->channels < 0)
        base->channels = MIX_DEFAULT_CHANNELS;
    if (OMG_ISNULL(this->mix.Mix_OpenAudioDevice))
        res = this->mix.Mix_OpenAudio(base->freq, base->use_float32 ? AUDIO_F32SYS : MIX_DEFAULT_FORMAT, base->channels, base->chunk_size);
    else
        res = this->mix.Mix_OpenAudioDevice(
            base->freq,
            base->use_float32 ? AUDIO_F32SYS : MIX_DEFAULT_FORMAT,
            base->channels,
            base->chunk_size,
            (OMG_ISNOTNULL(base->dev_name) && !omg_string_ensure_null((OMG_String*)base->dev_name)) ? base->dev_name->ptr : NULL,
            (base->allow_channels_change ? SDL_AUDIO_ALLOW_CHANNELS_CHANGE : 0) |
            (base->allow_format_change ? SDL_AUDIO_ALLOW_FORMAT_CHANGE : 0) |
            (base->allow_freq_change ? SDL_AUDIO_ALLOW_FREQUENCY_CHANGE : 0) |
            (base->allow_samples_change ? SDL_AUDIO_ALLOW_SAMPLES_CHANGE : 0)
        );
    if (res < 0) {
        _OMG_LOG_ERROR(omg_base, "Failed to open SDL2_mixer audio device (", MIX_GETERROR(), ")");
        this->mix.Mix_Quit();
        omg_sdl2_mixer_dll_free(&this->mix);
        return true;
    }
    if (MIX_CHANNELS != OMG_MAX_PLAYING_SOUND)
        this->mix.Mix_AllocateChannels(OMG_MAX_PLAYING_SOUND);
    this->mix.Mix_QuerySpec(&base->freq, NULL, &base->channels);
    omg_base->std->memset(this->play_cache, 0, sizeof(OMG_SoundSdl2*) * OMG_MAX_PLAYING_SOUND);
    this->supports_get_pos = OMG_ISNOTNULL(this->mix.Mix_GetMusicPosition);
    // this->supports_get_pos = false;
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_audio_sdl2_destroy;
    base->update = omg_audio_sdl2_update;
    base->mus_from_fp = omg_audio_sdl2_mus_from_fp;
    base->mus_from_mem = omg_audio_sdl2_mus_from_mem;
    base->mus_from_file = omg_audio_sdl2_mus_from_file;
    base->mus_destroy = omg_audio_sdl2_mus_destroy;
    base->mus_play = omg_audio_sdl2_mus_play;
    base->mus_set_volume = omg_audio_sdl2_mus_set_volume;
    base->mus_stop = omg_audio_sdl2_mus_stop;
    base->mus_pause = omg_audio_sdl2_mus_pause;
    base->mus_get_pos = omg_audio_sdl2_mus_get_pos;
    base->mus_set_pos = omg_audio_sdl2_mus_set_pos;
    base->snd_from_fp = omg_audio_sdl2_snd_from_fp;
    base->snd_from_mem = omg_audio_sdl2_snd_from_mem;
    base->snd_destroy = omg_audio_sdl2_snd_destroy;
    base->snd_play = omg_audio_sdl2_snd_play;
    base->snd_stop = omg_audio_sdl2_snd_stop;
    base->snd_pause = omg_audio_sdl2_snd_pause;
    base->snd_set_volume = omg_audio_sdl2_snd_set_volume;
    base->snd_set_panning = omg_audio_sdl2_snd_set_panning;
    OMG_END_POINTER_CAST();
    base->type = OMG_AUDIO_TYPE_SDL2;
    cur_audio_cache = this;
    base->inited = true;
    return false;
}
#endif
