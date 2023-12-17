#pragma once
#include <omega/ostd.h>

typedef struct {
    void* omg;
    void* data;
    uint64_t time;
} OMG_EventBase;

typedef struct {
    OMG_EventBase parent;
} OMG_EventQuit;
