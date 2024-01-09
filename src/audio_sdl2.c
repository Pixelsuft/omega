#include <omega/audio_sdl2.h>

#if OMG_SUPPORT_SDL2_MIXER
#include <omega/omega.h>
#define base ((OMG_Audio*)this)
#define mus_base ((OMG_Music*)mus)
#define omg_base ((OMG_Omega*)base->omg)
#define MIX_GETERROR() (OMG_ISNULL(this->sdl2) ? "" : this->sdl2->SDL_GetError())
#define MUS_IS_PLAYING() (cur_mus_cache == mus->mus)

static Mix_Music* cur_mus_cache = NULL;

bool omg_audio_sdl2_destroy(OMG_AudioSdl2* this) {
    if (!base->inited)
        return false;
    bool res = false;
    this->mix.Mix_CloseAudio();
    this->mix.Mix_Quit();
    res = omg_sdl2_mixer_dll_free(&this->mix) || res;
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

OMG_MusicSdl2* omg_audio_sdl2_mus_from_fp(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, const OMG_String* path) {
    if (omg_string_ensure_null((OMG_String*)path))
        return NULL;
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicSdl2));
        if (OMG_ISNULL(mus))
            return NULL;
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    mus->mus = this->mix.Mix_LoadMUS(path->ptr);
    if (OMG_ISNULL(mus->mus)) {
        omg_audio_mus_destroy(base, mus_base);
        _OMG_LOG_ERROR(omg_base, "Failed to open music ", path->ptr, " (", MIX_GETERROR(), ")");
        return NULL;
    }
    mus->vol_cache = MIX_MAX_VOLUME;
    return mus;
}

bool omg_audio_sdl2_mus_play(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, int loops, double pos, double fade_in) {
    _OMG_NULL_MUS_CHECK();
    int res;
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
    cur_mus_cache = mus->mus;
    this->mix.Mix_VolumeMusic(mus->vol_cache);
    return false;
}

bool omg_audio_sdl2_mus_set_volume(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, float volume) {
    _OMG_NULL_MUS_CHECK();
    mus->vol_cache = (int)(volume * (float)MIX_MAX_VOLUME);
    if (MUS_IS_PLAYING())
        this->mix.Mix_VolumeMusic(mus->vol_cache);
    return false;
}

bool omg_audio_sdl2_mus_stop(OMG_AudioSdl2* this, OMG_MusicSdl2* mus) {
    _OMG_NULL_MUS_CHECK();
    if (MUS_IS_PLAYING())
        this->mix.Mix_HaltMusic();
    return false;
}

bool omg_audio_sdl2_init(OMG_AudioSdl2* this) {
    omg_audio_init(base);
    if (omg_sdl2_mixer_dll_load(&this->mix, omg_base->sdl2_mixer_dll_path)) {
        _OMG_LOG_ERROR(omg_base, "Failed to load SDL2_mixer dll");
        return true;
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
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_audio_sdl2_destroy;
    base->mus_from_fp = omg_audio_sdl2_mus_from_fp;
    base->mus_destroy = omg_audio_sdl2_mus_destroy;
    base->mus_play = omg_audio_sdl2_mus_play;
    base->mus_set_volume = omg_audio_sdl2_mus_set_volume;
    base->mus_stop = omg_audio_sdl2_mus_stop;
    OMG_END_POINTER_CAST();
    base->type = OMG_AUDIO_TYPE_SDL2;
    base->inited = true;
    return false;
}
#endif
