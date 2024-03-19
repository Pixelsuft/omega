#pragma once
#include <omega/omega.h>

typedef struct {
    OMG_Texture* tex[32];
    OMG_Surface* surf[32];
    OMG_Thread* thr;
    void* _app;
    int total_count;
    int img_count;
    int tex_count;
    int progress;
    bool loaded_images;
    bool thread_safe;
    bool finished;
} Loader;

void loader_run(Loader* this);
void loader_init(Loader* this, void* _app);
void loader_update(Loader* this);
void loader_clean(Loader* this);
