#pragma once
#include <omega/omega.h>

typedef struct {
    OMG_Texture* tex[32];
    OMG_Thread* thr;
    void* _app;
    int total_count;
    int img_count;
    int progress;
} Loader;

void loader_run(Loader* this);
void loader_init(Loader* this, void* _app);
void loader_clean(Loader* this);
