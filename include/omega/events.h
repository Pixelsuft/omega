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
} OMG_EventClose;

typedef struct {
    OMG_Event parent;
    void* win;
} OMG_EventExpose;

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

typedef struct {
    OMG_Event parent;
    void* win;
    int change;
    bool allow;
} OMG_EventStateChanging;

typedef struct {
    OMG_Event parent;
    void* win;
    bool is_focused;
} OMG_EventFocusChange;

typedef struct {
    OMG_Event parent;
    void* win;
    OMG_FPoint pos;
    OMG_FPoint rel;
    uint32_t id;
    uint32_t state;
    bool is_emulated;
    uint8_t clicks;
} OMG_EventMouseMove;

typedef struct {
    OMG_Event parent;
    void* win;
    OMG_FPoint pos;
    uint32_t state;
    uint32_t id;
    bool is_emulated;
    bool is_pressed;
    uint8_t clicks;
    uint8_t button;
} OMG_EventMouseButton;

typedef struct {
    OMG_Event parent;
    void* win;
    OMG_FPoint mouse_pos;
    OMG_FPoint rel;
    uint32_t state;
    uint32_t id;
    bool is_emulated;
} OMG_EventMouseWheel;

typedef struct {
    OMG_Event parent;
    void* win;
    OMG_FPoint pos;
    uint32_t state;
    bool is_focused;
} OMG_EventMouseFocus;

typedef struct {
    OMG_Event parent;
    void* win;
    uint32_t code;
    uint32_t sym;
    uint16_t mod;
    bool is_repeated;
    bool is_pressed;
} OMG_EventKeyboard;

typedef struct {
    OMG_Event parent;
    void* win;
    OMG_String* text;
} OMG_EventTextInput;
