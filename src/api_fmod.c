#include <omega/api_fmod.h>
#if OMG_SUPPORT_FMOD
#if OMG_FMOD_DYNAMIC
#include <omega/api_static.h>
#define LOAD_REQUIRED(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#endif

bool omg_fmod_dll_load(OMG_Fmod* this, const OMG_String* dll_path) {
#if OMG_SDL2_MIXER_DYNAMIC
    if (OMG_ISNULL(dll_path))
#if OMG_IS_WIN
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("fmod.dll"), L"fmod.dll");
#else
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("libfmod.so"), NULL);
#endif
    else
        this->handle = omg_static_lib_load(dll_path, NULL);
    if (OMG_ISNULL(this->handle))
        return true;
#else
    OMG_UNUSED(dll_path);
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED(FMOD_System_Create);
    LOAD_REQUIRED(FMOD_System_Release);
    LOAD_REQUIRED(FMOD_System_SetSoftwareChannels);
    LOAD_REQUIRED(FMOD_System_SetSoftwareFormat);
    LOAD_REQUIRED(FMOD_System_GetSoftwareFormat);
    LOAD_REQUIRED(FMOD_System_Init);
    LOAD_REQUIRED(FMOD_System_Close);
    LOAD_REQUIRED(FMOD_System_Update);
    LOAD_REQUIRED(FMOD_System_GetVersion);
    LOAD_REQUIRED(FMOD_System_CreateSound);
    LOAD_REQUIRED(FMOD_System_CreateStream);
    LOAD_REQUIRED(FMOD_System_PlaySound);
    LOAD_REQUIRED(FMOD_System_GetChannel);
    LOAD_REQUIRED(FMOD_System_SetReverbProperties);
    LOAD_REQUIRED(FMOD_System_GetReverbProperties);
    LOAD_REQUIRED(FMOD_Sound_Release);
    LOAD_REQUIRED(FMOD_Sound_GetName);
    LOAD_REQUIRED(FMOD_Sound_GetLength);
    LOAD_REQUIRED(FMOD_Sound_GetFormat);
    LOAD_REQUIRED(FMOD_Sound_GetNumTags);
    LOAD_REQUIRED(FMOD_Sound_GetTag);
    LOAD_REQUIRED(FMOD_Sound_SetMode);
    LOAD_REQUIRED(FMOD_Sound_GetMode);
    LOAD_REQUIRED(FMOD_Sound_SetLoopCount);
    LOAD_REQUIRED(FMOD_Sound_GetLoopCount);
    LOAD_REQUIRED(FMOD_Sound_GetMusicNumChannels);
    LOAD_REQUIRED(FMOD_Sound_SetMusicChannelVolume);
    LOAD_REQUIRED(FMOD_Sound_GetMusicChannelVolume);
    LOAD_REQUIRED(FMOD_Sound_SetMusicSpeed);
    LOAD_REQUIRED(FMOD_Sound_GetMusicSpeed);
    LOAD_REQUIRED(FMOD_Channel_Stop);
    LOAD_REQUIRED(FMOD_Channel_SetPaused);
    LOAD_REQUIRED(FMOD_Channel_GetPaused);
    LOAD_REQUIRED(FMOD_Channel_SetVolume);
    LOAD_REQUIRED(FMOD_Channel_GetVolume);
    LOAD_REQUIRED(FMOD_Channel_SetPitch);
    LOAD_REQUIRED(FMOD_Channel_GetPitch);
    LOAD_REQUIRED(FMOD_Channel_SetMute);
    LOAD_REQUIRED(FMOD_Channel_GetMute);
    LOAD_REQUIRED(FMOD_Channel_SetReverbProperties);
    LOAD_REQUIRED(FMOD_Channel_GetReverbProperties);
    LOAD_REQUIRED(FMOD_Channel_SetLowPassGain);
    LOAD_REQUIRED(FMOD_Channel_GetLowPassGain);
    LOAD_REQUIRED(FMOD_Channel_SetMode);
    LOAD_REQUIRED(FMOD_Channel_GetMode);
    LOAD_REQUIRED(FMOD_Channel_SetCallback);
    LOAD_REQUIRED(FMOD_Channel_IsPlaying);
    LOAD_REQUIRED(FMOD_Channel_SetPan);
    LOAD_REQUIRED(FMOD_Channel_SetUserData);
    LOAD_REQUIRED(FMOD_Channel_GetUserData);
    LOAD_REQUIRED(FMOD_Channel_SetFrequency);
    LOAD_REQUIRED(FMOD_Channel_GetFrequency);
    LOAD_REQUIRED(FMOD_Channel_SetPosition);
    LOAD_REQUIRED(FMOD_Channel_GetPosition);
    LOAD_REQUIRED(FMOD_Channel_SetLoopCount);
    LOAD_REQUIRED(FMOD_Channel_GetLoopCount);
    LOAD_REQUIRED(FMOD_Channel_GetCurrentSound);
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_fmod_dll_free(OMG_Fmod* this) {
#if OMG_SDL2_MIXER_DYNAMIC
    return omg_static_lib_free(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}
#endif
