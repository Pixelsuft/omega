#include <omega/keyboard.h>

// Thanks to SDL2 for 99.9% of code

#define _OMG_SCANCODE_MAP NULL, \
    NULL, \
    NULL, \
    NULL, \
    "A", \
    "B", \
    "C", \
    "D", \
    "E", \
    "F", \
    "G", \
    "H", \
    "I", \
    "J", \
    "K", \
    "L", \
    "M", \
    "N", \
    "O", \
    "P", \
    "Q", \
    "R", \
    "S", \
    "T", \
    "U", \
    "V", \
    "W", \
    "X", \
    "Y", \
    "Z", \
    "1", \
    "2", \
    "3", \
    "4", \
    "5", \
    "6", \
    "7", \
    "8", \
    "9", \
    "0", \
    "Return", \
    "Escape", \
    "Backspace", \
    "Tab", \
    "Space", \
    "-", \
    "=", \
    "[", \
    "]", \
    "\\", \
    "#", \
    ";", \
    "'", \
    "`", \
    ",", \
    ".", \
    "/", \
    "CapsLock", \
    "F1", \
    "F2", \
    "F3", \
    "F4", \
    "F5", \
    "F6", \
    "F7", \
    "F8", \
    "F9", \
    "F10", \
    "F11", \
    "F12", \
    "PrintScreen", \
    "ScrollLock", \
    "Pause", \
    "Insert", \
    "Home", \
    "PageUp", \
    "Delete", \
    "End", \
    "PageDown", \
    "Right", \
    "Left", \
    "Down", \
    "Up", \
    "Numlock", \
    "Keypad /", \
    "Keypad *", \
    "Keypad -", \
    "Keypad +", \
    "Keypad Enter", \
    "Keypad 1", \
    "Keypad 2", \
    "Keypad 3", \
    "Keypad 4", \
    "Keypad 5", \
    "Keypad 6", \
    "Keypad 7", \
    "Keypad 8", \
    "Keypad 9", \
    "Keypad 0", \
    "Keypad .", \
    NULL, \
    "Application", \
    "Power", \
    "Keypad =", \
    "F13", \
    "F14", \
    "F15", \
    "F16", \
    "F17", \
    "F18", \
    "F19", \
    "F20", \
    "F21", \
    "F22", \
    "F23", \
    "F24", \
    "Execute", \
    "Help", \
    "Menu", \
    "Select", \
    "Stop", \
    "Again", \
    "Undo", \
    "Cut", \
    "Copy", \
    "Paste", \
    "Find", \
    "Mute", \
    "VolumeUp", \
    "VolumeDown", \
    NULL, \
    NULL, \
    NULL, \
    "Keypad ,", \
    "Keypad = (AS400)", \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    "AltErase", \
    "SysReq", \
    "Cancel", \
    "Clear", \
    "Prior", \
    "Return", \
    "Separator", \
    "Out", \
    "Oper", \
    "Clear / Again", \
    "CrSel", \
    "ExSel", \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    "Keypad 00", \
    "Keypad 000", \
    "ThousandsSeparator", \
    "DecimalSeparator", \
    "CurrencyUnit", \
    "CurrencySubUnit", \
    "Keypad (", \
    "Keypad )", \
    "Keypad {", \
    "Keypad }", \
    "Keypad Tab", \
    "Keypad Backspace", \
    "Keypad A", \
    "Keypad B", \
    "Keypad C", \
    "Keypad D", \
    "Keypad E", \
    "Keypad F", \
    "Keypad XOR", \
    "Keypad ^", \
    "Keypad %", \
    "Keypad <", \
    "Keypad >", \
    "Keypad &", \
    "Keypad &&", \
    "Keypad |", \
    "Keypad ||", \
    "Keypad :", \
    "Keypad #", \
    "Keypad Space", \
    "Keypad @", \
    "Keypad !", \
    "Keypad MemStore", \
    "Keypad MemRecall", \
    "Keypad MemClear", \
    "Keypad MemAdd", \
    "Keypad MemSubtract", \
    "Keypad MemMultiply", \
    "Keypad MemDivide", \
    "Keypad +/-", \
    "Keypad Clear", \
    "Keypad ClearEntry", \
    "Keypad Binary", \
    "Keypad Octal", \
    "Keypad Decimal", \
    "Keypad Hexadecimal", \
    NULL, \
    NULL, \
    "Left Ctrl", \
    "Left Shift", \
    "Left Alt", \
    "Left GUI", \
    "Right Ctrl", \
    "Right Shift", \
    "Right Alt", \
    "Right GUI", \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    NULL, \
    "ModeSwitch", \
    "AudioNext", \
    "AudioPrev", \
    "AudioStop", \
    "AudioPlay", \
    "AudioMute", \
    "MediaSelect", \
    "WWW", \
    "Mail", \
    "Calculator", \
    "Computer", \
    "AC Search", \
    "AC Home", \
    "AC Back", \
    "AC Forward", \
    "AC Stop", \
    "AC Refresh", \
    "AC Bookmarks", \
    "BrightnessDown", \
    "BrightnessUp", \
    "DisplaySwitch", \
    "KBDIllumToggle", \
    "KBDIllumDown", \
    "KBDIllumUp", \
    "Eject", \
    "Sleep", \
    "App1", \
    "App2", \
    "AudioRewind", \
    "AudioFastForward", \
    "SoftLeft", \
    "SoftRight", \
    "Call", \
    "EndCall" \

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

OMG_Keycode omg_keyboard_key_from_scancode(OMG_Scancode scancode) {
    return 0; // TODO
}

const OMG_String* omg_keyboard_name_from_key(OMG_Keycode key) {
    static char name[8];
    char* end;
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
