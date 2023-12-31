#include <omega/keyboard.h>

// Thanks to SDL2 for 99.9% of code

static const char* OMG_ScancodeNames[OMG_NUM_SCANCODES] = { _OMG_SCANCODE_MAP };

char* OMG_keyboard_UCS4ToUTF8(uint32_t ch, char* dst) {
    uint8_t *p = (uint8_t*)dst;
    if (ch <= 0x7F) {
        *p = (uint8_t)ch;
        ++dst;
    } else if (ch <= 0x7FF) {
        p[0] = 0xC0 | (uint8_t)((ch >> 6) & 0x1F);
        p[1] = 0x80 | (uint8_t)(ch & 0x3F);
        dst += 2;
    } else if (ch <= 0xFFFF) {
        p[0] = 0xE0 | (uint8_t)((ch >> 12) & 0x0F);
        p[1] = 0x80 | (uint8_t)((ch >> 6) & 0x3F);
        p[2] = 0x80 | (uint8_t)(ch & 0x3F);
        dst += 3;
    } else {
        p[0] = 0xF0 | (uint8_t)((ch >> 18) & 0x07);
        p[1] = 0x80 | (uint8_t)((ch >> 12) & 0x3F);
        p[2] = 0x80 | (uint8_t)((ch >> 6) & 0x3F);
        p[3] = 0x80 | (uint8_t)(ch & 0x3F);
        dst += 4;
    }
    return dst;
}

const OMG_String* omg_keyboard_name_from_scancode(OMG_Scancode scancode) {
    if (((int)scancode) < OMG_SCANCODE_UNKNOWN || scancode >= OMG_NUM_SCANCODES)
        return &OMG_STRING_MAKE_STATIC("");
    const char* res = OMG_ScancodeNames[scancode];
    if (OMG_ISNULL(res))
        return &OMG_STRING_MAKE_STATIC("");
    return &OMG_STRING_MAKE_STATIC((char*)res);
}

const OMG_String* omg_keyboard_name_from_key(OMG_Keycode key) {
    static char name[8];
    char *end;
    if (key & OMGK_SCANCODE_MASK) {
        return omg_keyboard_name_from_scancode((OMG_Scancode)(key & ~OMGK_SCANCODE_MASK));
    }
    switch (key) {
    case OMGK_RETURN:
        return omg_keyboard_name_from_scancode(OMG_SCANCODE_RETURN);
    case OMGK_ESCAPE:
        return omg_keyboard_name_from_scancode(OMG_SCANCODE_ESCAPE);
    case OMGK_BACKSPACE:
        return omg_keyboard_name_from_scancode(OMG_SCANCODE_BACKSPACE);
    case OMGK_TAB:
        return omg_keyboard_name_from_scancode(OMG_SCANCODE_TAB);
    case OMGK_SPACE:
        return omg_keyboard_name_from_scancode(OMG_SCANCODE_SPACE);
    case OMGK_DELETE:
        return omg_keyboard_name_from_scancode(OMG_SCANCODE_DELETE);
    default:
        if (key >= 'a' && key <= 'z') {
            key -= 32;
        }
        end = OMG_keyboard_UCS4ToUTF8((uint32_t)key, name);
        *end = '\0';
        return &OMG_STRING_MAKE_STATIC(name);
    }
    return &OMG_STRING_MAKE_STATIC("");
}
