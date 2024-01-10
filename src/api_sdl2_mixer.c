#include <omega/api_sdl2_mixer.h>
#if OMG_SUPPORT_SDL2_MIXER
#include <omega/ostd.h>
#if OMG_SDL2_MIXER_DYNAMIC
#include <omega/api_static.h>
#define LOAD_REQUIRED(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#if OMG_SDL2_MIXER_COMPAT_STATIC
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = NULL
#else
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = func_name
#endif
#endif

bool omg_sdl2_mixer_dll_load(OMG_Sdl2Mixer* this, const OMG_String* dll_path) {
#if OMG_SDL2_MIXER_DYNAMIC
    if (OMG_ISNULL(dll_path))
#if OMG_IS_WIN
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("SDL2_mixer.dll"), L"SDL2_mixer.dll");
#else
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("libSDL2_mixer.so"), NULL);
#endif
    else
        this->handle = omg_static_lib_load(dll_path, NULL);
    if (OMG_ISNULL(this->handle))
        return true;
#else
    OMG_UNUSED(dll_path);
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED(Mix_Linked_Version);
#if OMG_SDL2_MIXER_DYNAMIC
    const SDL_version* mix_ver = this->Mix_Linked_Version();
    this->ver.major = mix_ver->major;
    this->ver.minor = mix_ver->minor;
    this->ver.patch = mix_ver->patch;
#else
    this->ver.major = SDL_MIXER_MAJOR_VERSION;
    this->ver.minor = SDL_MIXER_MINOR_VERSION;
    this->ver.patch = SDL_MIXER_PATCHLEVEL;
#endif
    LOAD_REQUIRED(Mix_Init);
    LOAD_REQUIRED(Mix_Quit);
    LOAD_REQUIRED(Mix_OpenAudio);
    LOAD_REQUIRED(Mix_QuerySpec);
    LOAD_REQUIRED(Mix_AllocateChannels);
    LOAD_REQUIRED(Mix_LoadWAV_RW);
    LOAD_REQUIRED(Mix_LoadMUS);
    LOAD_REQUIRED(Mix_LoadMUS_RW);
    LOAD_REQUIRED(Mix_LoadMUSType_RW);
    LOAD_REQUIRED(Mix_QuickLoad_WAV);
    LOAD_REQUIRED(Mix_QuickLoad_RAW);
    LOAD_REQUIRED(Mix_FreeChunk);
    LOAD_REQUIRED(Mix_FreeMusic);
    LOAD_REQUIRED(Mix_GetNumChunkDecoders);
    LOAD_REQUIRED(Mix_GetChunkDecoder);
    LOAD_REQUIRED(Mix_HasChunkDecoder);
    LOAD_REQUIRED(Mix_GetNumMusicDecoders);
    LOAD_REQUIRED(Mix_GetMusicDecoder);
    LOAD_REQUIRED(Mix_HasMusicDecoder);
    LOAD_REQUIRED(Mix_GetMusicType);
    LOAD_REQUIRED(Mix_HookMusic);
    LOAD_REQUIRED(Mix_HookMusicFinished);
    LOAD_REQUIRED(Mix_GetMusicHookData);
    LOAD_REQUIRED(Mix_ChannelFinished);
    LOAD_REQUIRED(Mix_SetPanning);
    LOAD_REQUIRED(Mix_SetPosition);
    LOAD_REQUIRED(Mix_SetDistance);
    LOAD_REQUIRED(Mix_SetReverseStereo);
    LOAD_REQUIRED(Mix_ReserveChannels);
    LOAD_REQUIRED(Mix_PlayChannelTimed);
    LOAD_REQUIRED(Mix_PlayMusic);
    LOAD_REQUIRED(Mix_FadeInMusic);
    LOAD_REQUIRED(Mix_FadeInMusicPos);
    LOAD_REQUIRED(Mix_FadeInChannelTimed);
    LOAD_REQUIRED(Mix_Volume);
    LOAD_REQUIRED(Mix_VolumeChunk);
    LOAD_REQUIRED(Mix_VolumeMusic);
    LOAD_REQUIRED(Mix_HaltChannel);
    LOAD_REQUIRED(Mix_HaltMusic);
    LOAD_REQUIRED(Mix_ExpireChannel);
    LOAD_REQUIRED(Mix_FadeOutChannel);
    LOAD_REQUIRED(Mix_FadeOutMusic);
    LOAD_REQUIRED(Mix_FadingMusic);
    LOAD_REQUIRED(Mix_FadingChannel);
    LOAD_REQUIRED(Mix_Pause);
    LOAD_REQUIRED(Mix_Resume);
    LOAD_REQUIRED(Mix_Paused);
    LOAD_REQUIRED(Mix_PauseMusic);
    LOAD_REQUIRED(Mix_ResumeMusic);
    LOAD_REQUIRED(Mix_RewindMusic);
    LOAD_REQUIRED(Mix_PausedMusic);
    LOAD_REQUIRED(Mix_SetMusicPosition);
    LOAD_REQUIRED(Mix_GetMusicPosition);
    LOAD_REQUIRED(Mix_MusicDuration);
    LOAD_REQUIRED(Mix_Playing);
    LOAD_REQUIRED(Mix_PlayingMusic);
    LOAD_REQUIRED(Mix_SetMusicCMD);
    LOAD_REQUIRED(Mix_GetChunk);
    LOAD_REQUIRED(Mix_CloseAudio);
    LOAD_REQUIRED_COMPAT(Mix_OpenAudioDevice); // 2.0.2
    LOAD_REQUIRED_COMPAT(Mix_GetMusicTitle); // 2.6.0
    LOAD_REQUIRED_COMPAT(Mix_GetMusicTitleTag); // 2.6.0
    LOAD_REQUIRED_COMPAT(Mix_GetMusicArtistTag); // 2.6.0
    LOAD_REQUIRED_COMPAT(Mix_GetMusicAlbumTag); // 2.6.0
    LOAD_REQUIRED_COMPAT(Mix_GetMusicCopyrightTag); // 2.6.0
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_sdl2_mixer_dll_free(OMG_Sdl2Mixer* this) {
#if OMG_SDL2_MIXER_DYNAMIC
    return omg_static_lib_free(this->handle);
#else
    return false;
#endif
}
#endif
