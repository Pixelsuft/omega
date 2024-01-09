#include <omega/audio_sdl2.h>

#if OMG_SUPPORT_SDL2_MIXER
#include <omega/omega_sdl2.h>
#define base ((OMG_Audio*)this)
#define omg_base ((OMG_Omega*)base->omg)
#define MIX_GETERROR() ((omg_base->type == OMG_OMEGA_TYPE_SDL2) ? ((OMG_OmegaSdl2*)omg_base)->sdl2->SDL_GetError() : "")

bool omg_audio_sdl2_destroy(OMG_AudioSdl2* this) {
    if (!base->inited)
        return false;
    bool res = false;
    this->mix.Mix_CloseAudio();
    this->mix.Mix_Quit();
    res = omg_sdl2_mixer_dll_free(&this->mix) || res;
    return res;
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
    OMG_END_POINTER_CAST();
    base->type = OMG_AUDIO_TYPE_SDL2;
    base->inited = true;
    return false;
}
#endif
