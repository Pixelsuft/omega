#pragma once
#include <omega/ostd.h>
#define OMG_OMEGA_FROM_EVENT(event) ((OMG_Omega*)(((OMG_Event*)event)->omg))
#define OMG_WIN_FROM_EVENT(event) ((OMG_Window*)((event)->win))
#define OMG_ARG_FROM_EVENT(event) (((OMG_Event*)event)->data)

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
} OMG_EventLoopStop;

typedef struct {
    OMG_Event parent;
    void* win;
} OMG_EventPaint;

typedef struct {
    OMG_Event parent;
    void* win;
    OMG_FPoint size;
} OMG_EventResize;

typedef struct {
    OMG_Event parent;
    void* win;
    int prev_state;
    int change;
} OMG_EventStateChange;
