#include <omega/keyboard.h>

static const char* OMG_ScancodeNames[OMG_NUM_SCANCODES] = { _OMG_SCANCODE_MAP };

const OMG_String* omg_keyboard_name_from_scancode(OMG_Scancode scancode) {
    if (((int)scancode) < OMG_SCANCODE_UNKNOWN || scancode >= OMG_NUM_SCANCODES)
        return &OMG_STRING_MAKE_STATIC("");
    const char* res = OMG_ScancodeNames[scancode];
    if (OMG_ISNULL(res))
        return &OMG_STRING_MAKE_STATIC("");
    return &OMG_STRING_MAKE_STATIC((char*)res);
}
