#include <omega/audio_emscripten.h>

#if OMG_SUPPORT_EMSCRIPTEN_AUDIO
#include <omega/omega.h>
#include <emscripten.h>
#define base ((OMG_Audio*)this)
#define mus_base ((OMG_Music*)this)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_audio_emscripten_mus_destroy(OMG_AudioEm* this, OMG_MusicEm* mus) {
    if (OMG_ISNULL(mus) || (mus->id < 0)) {
        return false;
    }
    EM_ASM({
        if (window.em_audio[$0] == undefined)
            return;
        window.em_audio[$0].pause();
        window.em_audio[$0] = undefined;
    }, mus->id);
    mus->id = -1;
    return false;
}

// Async doen't work with big files
// https://stackoverflow.com/questions/37104199/how-to-await-for-a-callback-to-return
EM_JS(int, omg_emscripten_mus_load, (const char* str_ptr), {
    var log_str = UTF8ToString(str_ptr);
    console.log(log_str);
    window.log_str = log_str;
    var cur_audio = new Audio(log_str);
    var temp_em_audio_id = -1;
    for (var temp_em_audio_i = 0; temp_em_audio_i < window.em_audio.length; temp_em_audio_i++) {
        if (window.em_audio[temp_em_audio_i] == undefined) {
            temp_em_audio_id = temp_em_audio_i;
            break;
        }
    }
    if (temp_em_audio_id == -1)
        temp_em_audio_id = window.em_audio.length;
    window.em_audio[temp_em_audio_id] = cur_audio;
    return temp_em_audio_id;
});

OMG_MusicEm* omg_audio_emscripten_mus_from_mem(OMG_AudioEm* this, OMG_MusicEm* mus, const void* data, size_t size, int format) {
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicEm));
        if (OMG_ISNULL(mus))
            return (OMG_MusicEm*)omg_audio_dummy_mus_alloc(base, mus_base);
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    size_t str_size = OMG_B64_ENCODED_LEN(size) + 40;
    OMG_String inp_str;
    inp_str.ptr = (char*)data;
    inp_str.len = size;
    inp_str.size = size;
    inp_str.type = OMG_STRING_STATIC;
    OMG_String base_str;
    if (omg_string_init_dynamic(&base_str, NULL) || omg_string_resize(&base_str, str_size)) {
        omg_string_destroy(&base_str);
        if (mus_base->was_allocated)
            OMG_FREE(omg_base->mem, mus);
        return (OMG_MusicEm*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    base_str.len = 0;
    omg_string_add_char_p(&base_str, "data:audio/");
    OMG_UNUSED(format);
    omg_string_add_char_p(&base_str, "mp3"); // TODO: format detection
    omg_string_add_char_p(&base_str, ";base64,");
    if (omg_base64_encode(&inp_str, &base_str)->size == 0) {
        omg_string_destroy(&base_str);
        if (mus_base->was_allocated)
            OMG_FREE(omg_base->mem, mus);
        return (OMG_MusicEm*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    base_str.ptr[base_str.len] = '\0';
    mus->id = omg_emscripten_mus_load(base_str.ptr);
    omg_string_destroy(&base_str);
    return mus;
}

bool omg_audio_emscripten_mus_set_volume(OMG_AudioEm* this, OMG_MusicEm* mus, float volume) {
    EM_ASM({
        var em_cur_audio = window.em_audio[$0];
        if (em_cur_audio == undefined)
            return;
        em_cur_audio.volume = $1;
    }, mus->id, volume);
    return false;
}

bool omg_audio_emscripten_mus_stop(OMG_AudioEm* this, OMG_MusicEm* mus) {
    return false;
}

bool omg_audio_emscripten_mus_play(OMG_AudioEm* this, OMG_MusicEm* mus, int loops, double pos, double fade_in) {
    OMG_UNUSED(loops, fade_in);
    int res = EM_ASM_INT({
        var em_cur_audio = window.em_audio[$0];
        if (em_cur_audio == undefined || em_cur_audio.readyState < 4)
            return -1;
        em_cur_audio.currentTime = $1;
        em_cur_audio.play();
        return 0;
    }, mus->id, pos);
    if (res < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to play audio");
        return true;
    }
    return false;
}

bool omg_audio_emscripten_init(OMG_AudioEm* this) {
    omg_audio_init(base);
    EM_ASM({
        window.em_audio = [];
    });
    OMG_BEGIN_POINTER_CAST();
    base->mus_from_mem = omg_audio_emscripten_mus_from_mem;
    base->mus_destroy = omg_audio_emscripten_mus_destroy;
    base->mus_set_volume = omg_audio_emscripten_mus_set_volume;
    base->mus_stop = omg_audio_emscripten_mus_stop;
    base->mus_play = omg_audio_emscripten_mus_play;
    // Hacky
    base->snd_from_mem = omg_audio_emscripten_mus_from_mem;
    base->snd_destroy = omg_audio_emscripten_mus_destroy;
    base->snd_set_volume = omg_audio_emscripten_mus_set_volume;
    base->snd_stop = omg_audio_emscripten_mus_stop;
    base->snd_play = omg_audio_emscripten_mus_play;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
