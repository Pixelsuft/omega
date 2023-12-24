#pragma once
#include <omega/ostd.h>
#define OMG_FROM_EVENT(event) ((OMG_Omega*)(((OMG_Event*)event)->omg))

typedef struct {
    void* omg;
    void* data;
    uint64_t time;
} OMG_Event;

typedef struct {
    OMG_Event parent;
} OMG_EventQuit;

typedef struct {
    OMG_Event parent;
} OMG_EventUpdate;

typedef struct {
    OMG_Event parent;
    void* win;
} OMG_EventPaint;

typedef struct {
    OMG_Event parent;
} OMG_EventLoopStop;
