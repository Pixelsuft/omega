#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_SDL2_MIXER
#include <omega/entry.h>
#include <omega/api_sdl2.h>
#if !OMG_SDL2_MIXER_DYNAMIC
#if OMG_IS_VC || OMG_IS_UNIX || OMG_IS_EMSCRIPTEN || OMG_IS_ANDROID
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif
#endif

#if OMG_SDL2_MIXER_DYNAMIC
#define OMG_SDL2_MIXER_STD_PREFIX

typedef enum {
    MIX_INIT_FLAC   = 0x00000001,
    MIX_INIT_MOD    = 0x00000002,
    MIX_INIT_MP3    = 0x00000008,
    MIX_INIT_OGG    = 0x00000010,
    MIX_INIT_MID    = 0x00000020,
    MIX_INIT_OPUS   = 0x00000040,
    MIX_INIT_WAVPACK= 0x00000080
} MIX_InitFlags;

typedef struct Mix_Chunk {
    int allocated;
    uint8_t* abuf;
    uint32_t alen;
    uint8_t volume;
} Mix_Chunk;

typedef enum {
    MIX_NO_FADING,
    MIX_FADING_OUT,
    MIX_FADING_IN
} Mix_Fading;

typedef enum {
    MUS_NONE,
    MUS_CMD,
    MUS_WAV,
    MUS_MOD,
    MUS_MID,
    MUS_OGG,
    MUS_MP3,
    MUS_MP3_MAD_UNUSED,
    MUS_FLAC,
    MUS_MODPLUG_UNUSED,
    MUS_OPUS,
    MUS_WAVPACK,
    MUS_GME
} Mix_MusicType;

typedef struct _Mix_Music Mix_Music;

#ifndef MIX_CHANNELS
#define MIX_CHANNELS    8
#endif

#define MIX_DEFAULT_FREQUENCY   44100
#define MIX_DEFAULT_FORMAT      AUDIO_S16SYS
#define MIX_DEFAULT_CHANNELS    2
#define MIX_MAX_VOLUME          SDL_MIX_MAXVOLUME 

#else
#define OMG_SDL2_MIXER_STD_PREFIX SDLCALL
#endif

typedef struct {
    void* handle;
    const SDL_version* OMG_SDL2_MIXER_STD_PREFIX (*Mix_Linked_Version)(void);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_Init)(int);
    void OMG_SDL2_MIXER_STD_PREFIX (*Mix_Quit)(void);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_OpenAudio)(int, uint16_t, int, int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_OpenAudioDevice)(int, uint16_t, int, int, const char*, int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_QuerySpec)(int*, uint16_t*, int*);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_AllocateChannels)(int);
    Mix_Chunk* OMG_SDL2_MIXER_STD_PREFIX (*Mix_LoadWAV_RW)(SDL_RWops*, int);
    Mix_Music* OMG_SDL2_MIXER_STD_PREFIX (*Mix_LoadMUS)(const char*);
    Mix_Music* OMG_SDL2_MIXER_STD_PREFIX (*Mix_LoadMUS_RW)(SDL_RWops*, int);
    Mix_Music* OMG_SDL2_MIXER_STD_PREFIX (*Mix_LoadMUSType_RW)(SDL_RWops*, Mix_MusicType, int);
    Mix_Chunk* OMG_SDL2_MIXER_STD_PREFIX (*Mix_QuickLoad_WAV)(uint8_t*);
    Mix_Chunk* OMG_SDL2_MIXER_STD_PREFIX (*Mix_QuickLoad_RAW)(uint8_t*, uint32_t);
    void OMG_SDL2_MIXER_STD_PREFIX (*Mix_FreeChunk)(Mix_Chunk*);
    void OMG_SDL2_MIXER_STD_PREFIX (*Mix_FreeMusic)(Mix_Music*);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_GetNumChunkDecoders)(void);
    const char* OMG_SDL2_MIXER_STD_PREFIX (*Mix_GetChunkDecoder)(int);
    SDL_bool OMG_SDL2_MIXER_STD_PREFIX (*Mix_HasChunkDecoder)(const char*);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_GetNumMusicDecoders)(void);
    const char* OMG_SDL2_MIXER_STD_PREFIX (*Mix_GetMusicDecoder)(int);
    SDL_bool OMG_SDL2_MIXER_STD_PREFIX (*Mix_HasMusicDecoder)(const char*);
    Mix_MusicType OMG_SDL2_MIXER_STD_PREFIX (*Mix_GetMusicType)(const Mix_Music*);
    const char* OMG_SDL2_MIXER_STD_PREFIX (*Mix_GetMusicTitle)(const Mix_Music*);
    const char* OMG_SDL2_MIXER_STD_PREFIX (*Mix_GetMusicTitleTag)(const Mix_Music*);
    const char* OMG_SDL2_MIXER_STD_PREFIX (*Mix_GetMusicArtistTag)(const Mix_Music*);
    const char* OMG_SDL2_MIXER_STD_PREFIX (*Mix_GetMusicAlbumTag)(const Mix_Music*);
    const char* OMG_SDL2_MIXER_STD_PREFIX (*Mix_GetMusicCopyrightTag)(const Mix_Music*);
    void OMG_SDL2_MIXER_STD_PREFIX (*Mix_HookMusic)(void (OMG_SDL2_MIXER_STD_PREFIX*)(void*, uint8_t*, int), void*);
    void OMG_SDL2_MIXER_STD_PREFIX (*Mix_HookMusicFinished)(void (OMG_SDL2_MIXER_STD_PREFIX *)(void));
    void* OMG_SDL2_MIXER_STD_PREFIX (*Mix_GetMusicHookData)(void);
    void OMG_SDL2_MIXER_STD_PREFIX (*Mix_ChannelFinished)(void (OMG_SDL2_MIXER_STD_PREFIX*)(int));
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_SetPanning)(int, uint8_t, uint8_t);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_SetPosition)(int, int16_t, uint8_t);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_SetDistance)(int, uint8_t);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_SetReverseStereo)(int, int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_ReserveChannels)(int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_PlayChannelTimed)(int, Mix_Chunk*, int, int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_PlayMusic)(Mix_Music*, int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_FadeInMusic)(Mix_Music*, int, int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_FadeInMusicPos)(Mix_Music*, int, int, double);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_FadeInChannelTimed)(int, Mix_Chunk*, int, int, int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_Volume)(int, int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_VolumeChunk)(Mix_Chunk*, int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_VolumeMusic)(int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_HaltChannel)(int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_HaltMusic)(void);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_ExpireChannel)(int, int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_FadeOutChannel)(int, int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_FadeOutMusic)(int);
    Mix_Fading OMG_SDL2_MIXER_STD_PREFIX (*Mix_FadingMusic)(void);
    Mix_Fading OMG_SDL2_MIXER_STD_PREFIX (*Mix_FadingChannel)(int);
    void OMG_SDL2_MIXER_STD_PREFIX (*Mix_Pause)(int);
    void OMG_SDL2_MIXER_STD_PREFIX (*Mix_Resume)(int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_Paused)(int);
    void OMG_SDL2_MIXER_STD_PREFIX (*Mix_PauseMusic)(void);
    void OMG_SDL2_MIXER_STD_PREFIX (*Mix_ResumeMusic)(void);
    void OMG_SDL2_MIXER_STD_PREFIX (*Mix_RewindMusic)(void);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_PausedMusic)(void);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_SetMusicPosition)(double);
    double OMG_SDL2_MIXER_STD_PREFIX (*Mix_GetMusicPosition)(Mix_Music*);
    double OMG_SDL2_MIXER_STD_PREFIX (*Mix_MusicDuration)(Mix_Music*);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_Playing)(int);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_PlayingMusic)(void);
    int OMG_SDL2_MIXER_STD_PREFIX (*Mix_SetMusicCMD)(const char*);
    Mix_Chunk* OMG_SDL2_MIXER_STD_PREFIX (*Mix_GetChunk)(int);
    void OMG_SDL2_MIXER_STD_PREFIX (*Mix_CloseAudio)(void);
    SDL_version ver;
} OMG_Sdl2Mixer;

OMG_API bool omg_sdl2_mixer_dll_load(OMG_Sdl2Mixer* this, const OMG_String* dll_path);
OMG_API bool omg_sdl2_mixer_dll_free(OMG_Sdl2Mixer* this);
#endif
