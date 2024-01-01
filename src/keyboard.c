#include <omega/keyboard.h>
#include <omega/omega.h>

// Thanks to SDL2 for 99.9% of code

// TODO: probably store in omg struct???

static const char* OMG_ScancodeNames[OMG_NUM_SCANCODES] = { 
    NULL,
    NULL,
    NULL,
    NULL,
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "Return",
    "Escape",
    "Backspace",
    "Tab",
    "Space",
    "-",
    "=",
    "[",
    "]",
    "\\",
    "#",
    ";",
    "'",
    "`",
    ",",
    ".",
    "/",
    "CapsLock",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "PrintScreen",
    "ScrollLock",
    "Pause",
    "Insert",
    "Home",
    "PageUp",
    "Delete",
    "End",
    "PageDown",
    "Right",
    "Left",
    "Down",
    "Up",
    "Numlock",
    "Keypad /",
    "Keypad *",
    "Keypad -",
    "Keypad +",
    "Keypad Enter",
    "Keypad 1",
    "Keypad 2",
    "Keypad 3",
    "Keypad 4",
    "Keypad 5",
    "Keypad 6",
    "Keypad 7",
    "Keypad 8",
    "Keypad 9",
    "Keypad 0",
    "Keypad .",
    NULL,
    "Application",
    "Power",
    "Keypad =",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "Execute",
    "Help",
    "Menu",
    "Select",
    "Stop",
    "Again",
    "Undo",
    "Cut",
    "Copy",
    "Paste",
    "Find",
    "Mute",
    "VolumeUp",
    "VolumeDown",
    NULL,
    NULL,
    NULL,
    "Keypad ,",
    "Keypad = (AS400)",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "AltErase",
    "SysReq",
    "Cancel",
    "Clear",
    "Prior",
    "Return",
    "Separator",
    "Out",
    "Oper",
    "Clear / Again",
    "CrSel",
    "ExSel",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Keypad 00",
    "Keypad 000",
    "ThousandsSeparator",
    "DecimalSeparator",
    "CurrencyUnit",
    "CurrencySubUnit",
    "Keypad (",
    "Keypad )",
    "Keypad {",
    "Keypad }",
    "Keypad Tab",
    "Keypad Backspace",
    "Keypad A",
    "Keypad B",
    "Keypad C",
    "Keypad D",
    "Keypad E",
    "Keypad F",
    "Keypad XOR",
    "Keypad ^",
    "Keypad %",
    "Keypad <",
    "Keypad >",
    "Keypad &",
    "Keypad &&",
    "Keypad |",
    "Keypad ||",
    "Keypad :",
    "Keypad #",
    "Keypad Space",
    "Keypad @",
    "Keypad !",
    "Keypad MemStore",
    "Keypad MemRecall",
    "Keypad MemClear",
    "Keypad MemAdd",
    "Keypad MemSubtract",
    "Keypad MemMultiply",
    "Keypad MemDivide",
    "Keypad +/-",
    "Keypad Clear",
    "Keypad ClearEntry",
    "Keypad Binary",
    "Keypad Octal",
    "Keypad Decimal",
    "Keypad Hexadecimal",
    NULL,
    NULL,
    "Left Ctrl",
    "Left Shift",
    "Left Alt",
    "Left GUI",
    "Right Ctrl",
    "Right Shift",
    "Right Alt",
    "Right GUI",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "ModeSwitch",
    "AudioNext",
    "AudioPrev",
    "AudioStop",
    "AudioPlay",
    "AudioMute",
    "MediaSelect",
    "WWW",
    "Mail",
    "Calculator",
    "Computer",
    "AC Search",
    "AC Home",
    "AC Back",
    "AC Forward",
    "AC Stop",
    "AC Refresh",
    "AC Bookmarks",
    "BrightnessDown",
    "BrightnessUp",
    "DisplaySwitch",
    "KBDIllumToggle",
    "KBDIllumDown",
    "KBDIllumUp",
    "Eject",
    "Sleep",
    "App1",
    "App2",
    "AudioRewind",
    "AudioFastForward",
    "SoftLeft",
    "SoftRight",
    "Call",
    "EndCall"
 };

 static const OMG_Keycode OMG_default_keymap[OMG_NUM_SCANCODES] = {
	0,
	0,
	0,
	0,
	'a',
	'b',
	'c',
	'd',
	'e',
	'f',
	'g',
	'h',
	'i',
	'j',
	'k',
	'l',
	'm',
	'n',
	'o',
	'p',
	'q',
	'r',
	's',
	't',
	'u',
	'v',
	'w',
	'x',
	'y',
	'z',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'0',
	OMGK_RETURN,
	OMGK_ESCAPE,
	OMGK_BACKSPACE,
	OMGK_TAB,
	OMGK_SPACE,
	'-',
	'=',
	'[',
	']',
	'\\',
	'#',
	';',
	'\'',
	'`',
	',',
	'.',
	'/',
	OMGK_CAPSLOCK,
	OMGK_F1,
	OMGK_F2,
	OMGK_F3,
	OMGK_F4,
	OMGK_F5,
	OMGK_F6,
	OMGK_F7,
	OMGK_F8,
	OMGK_F9,
	OMGK_F10,
	OMGK_F11,
	OMGK_F12,
	OMGK_PRINTSCREEN,
	OMGK_SCROLLLOCK,
	OMGK_PAUSE,
	OMGK_INSERT,
	OMGK_HOME,
	OMGK_PAGEUP,
	OMGK_DELETE,
	OMGK_END,
	OMGK_PAGEDOWN,
	OMGK_RIGHT,
	OMGK_LEFT,
	OMGK_DOWN,
	OMGK_UP,
	OMGK_NUMLOCKCLEAR,
	OMGK_KP_DIVIDE,
	OMGK_KP_MULTIPLY,
	OMGK_KP_MINUS,
	OMGK_KP_PLUS,
	OMGK_KP_ENTER,
	OMGK_KP_1,
	OMGK_KP_2,
	OMGK_KP_3,
	OMGK_KP_4,
	OMGK_KP_5,
	OMGK_KP_6,
	OMGK_KP_7,
	OMGK_KP_8,
	OMGK_KP_9,
	OMGK_KP_0,
	OMGK_KP_PERIOD,
	0,
	OMGK_APPLICATION,
	OMGK_POWER,
	OMGK_KP_EQUALS,
	OMGK_F13,
	OMGK_F14,
	OMGK_F15,
	OMGK_F16,
	OMGK_F17,
	OMGK_F18,
	OMGK_F19,
	OMGK_F20,
	OMGK_F21,
	OMGK_F22,
	OMGK_F23,
	OMGK_F24,
	OMGK_EXECUTE,
	OMGK_HELP,
	OMGK_MENU,
	OMGK_SELECT,
	OMGK_STOP,
	OMGK_AGAIN,
	OMGK_UNDO,
	OMGK_CUT,
	OMGK_COPY,
	OMGK_PASTE,
	OMGK_FIND,
	OMGK_MUTE,
	OMGK_VOLUMEUP,
	OMGK_VOLUMEDOWN,
	0,
	0,
	0,
	OMGK_KP_COMMA,
	OMGK_KP_EQUALSAS400,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	OMGK_ALTERASE,
	OMGK_SYSREQ,
	OMGK_CANCEL,
	OMGK_CLEAR,
	OMGK_PRIOR,
	OMGK_RETURN2,
	OMGK_SEPARATOR,
	OMGK_OUT,
	OMGK_OPER,
	OMGK_CLEARAGAIN,
	OMGK_CRSEL,
	OMGK_EXSEL,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	OMGK_KP_00,
	OMGK_KP_000,
	OMGK_THOUSANDSSEPARATOR,
	OMGK_DECIMALSEPARATOR,
	OMGK_CURRENCYUNIT,
	OMGK_CURRENCYSUBUNIT,
	OMGK_KP_LEFTPAREN,
	OMGK_KP_RIGHTPAREN,
	OMGK_KP_LEFTBRACE,
	OMGK_KP_RIGHTBRACE,
	OMGK_KP_TAB,
	OMGK_KP_BACKSPACE,
	OMGK_KP_A,
	OMGK_KP_B,
	OMGK_KP_C,
	OMGK_KP_D,
	OMGK_KP_E,
	OMGK_KP_F,
	OMGK_KP_XOR,
	OMGK_KP_POWER,
	OMGK_KP_PERCENT,
	OMGK_KP_LESS,
	OMGK_KP_GREATER,
	OMGK_KP_AMPERSAND,
	OMGK_KP_DBLAMPERSAND,
	OMGK_KP_VERTICALBAR,
	OMGK_KP_DBLVERTICALBAR,
	OMGK_KP_COLON,
	OMGK_KP_HASH,
	OMGK_KP_SPACE,
	OMGK_KP_AT,
	OMGK_KP_EXCLAM,
	OMGK_KP_MEMSTORE,
	OMGK_KP_MEMRECALL,
	OMGK_KP_MEMCLEAR,
	OMGK_KP_MEMADD,
	OMGK_KP_MEMSUBTRACT,
	OMGK_KP_MEMMULTIPLY,
	OMGK_KP_MEMDIVIDE,
	OMGK_KP_PLUSMINUS,
	OMGK_KP_CLEAR,
	OMGK_KP_CLEARENTRY,
	OMGK_KP_BINARY,
	OMGK_KP_OCTAL,
	OMGK_KP_DECIMAL,
	OMGK_KP_HEXADECIMAL,
	0,
	0,
	OMGK_LCTRL,
	OMGK_LSHIFT,
	OMGK_LALT,
	OMGK_LGUI,
	OMGK_RCTRL,
	OMGK_RSHIFT,
	OMGK_RALT,
	OMGK_RGUI,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	OMGK_MODE,
	OMGK_AUDIONEXT,
	OMGK_AUDIOPREV,
	OMGK_AUDIOSTOP,
	OMGK_AUDIOPLAY,
	OMGK_AUDIOMUTE,
	OMGK_MEDIASELECT,
	OMGK_WWW,
	OMGK_MAIL,
	OMGK_CALCULATOR,
	OMGK_COMPUTER,
	OMGK_AC_SEARCH,
	OMGK_AC_HOME,
	OMGK_AC_BACK,
	OMGK_AC_FORWARD,
	OMGK_AC_STOP,
	OMGK_AC_REFRESH,
	OMGK_AC_BOOKMARKS,
	OMGK_BRIGHTNESSDOWN,
	OMGK_BRIGHTNESSUP,
	OMGK_DISPLAYSWITCH,
	OMGK_KBDILLUMTOGGLE,
	OMGK_KBDILLUMDOWN,
	OMGK_KBDILLUMUP,
	OMGK_EJECT,
	OMGK_SLEEP,
	OMGK_APP1,
	OMGK_APP2,
	OMGK_AUDIOREWIND,
	OMGK_AUDIOFASTFORWARD,
	OMGK_SOFTLEFT,
	OMGK_SOFTRIGHT,
	OMGK_CALL,
	OMGK_ENDCALL
};

static const OMG_Keycode* keymap = NULL;

void omg_keyboard_init_keymap(void* omg) {
    OMG_Omega* this = (OMG_Omega*)omg;
    keymap = this->keymap;
    this->std->memcpy((void*)this->keymap, OMG_default_keymap, sizeof(OMG_Keycode) * OMG_NUM_SCANCODES);
}

char* omg_keyboard_UCS4ToUTF8(uint32_t ch, char* dst) {
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
    // TODO: update keymap
    return keymap[scancode];
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
            end = omg_keyboard_UCS4ToUTF8((uint32_t)key, name);
            *end = '\0';
            return &OMG_STRING_MAKE_STATIC(name);
    }
    return &OMG_STRING_MAKE_STATIC("");
}
