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
} OMG_Sdl2Mixer;

OMG_API bool omg_sdl2_mixer_dll_load(OMG_Sdl2Mixer* this, const OMG_String* dll_path);
OMG_API bool omg_sdl2_mixer_dll_free(OMG_Sdl2Mixer* this);
#endif
