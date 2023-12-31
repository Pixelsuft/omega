#pragma once
#include <omega/ostd.h>

/* Yes, I like SDL */

#define OMGK_SCANCODE_MASK (1 << 30)
#define OMG_SCANCODE_TO_KEYCODE(X)  (X | OMGK_SCANCODE_MASK)

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

typedef enum
{
    OMG_SCANCODE_UNKNOWN = 0,
    OMG_SCANCODE_A = 4,
    OMG_SCANCODE_B = 5,
    OMG_SCANCODE_C = 6,
    OMG_SCANCODE_D = 7,
    OMG_SCANCODE_E = 8,
    OMG_SCANCODE_F = 9,
    OMG_SCANCODE_G = 10,
    OMG_SCANCODE_H = 11,
    OMG_SCANCODE_I = 12,
    OMG_SCANCODE_J = 13,
    OMG_SCANCODE_K = 14,
    OMG_SCANCODE_L = 15,
    OMG_SCANCODE_M = 16,
    OMG_SCANCODE_N = 17,
    OMG_SCANCODE_O = 18,
    OMG_SCANCODE_P = 19,
    OMG_SCANCODE_Q = 20,
    OMG_SCANCODE_R = 21,
    OMG_SCANCODE_S = 22,
    OMG_SCANCODE_T = 23,
    OMG_SCANCODE_U = 24,
    OMG_SCANCODE_V = 25,
    OMG_SCANCODE_W = 26,
    OMG_SCANCODE_X = 27,
    OMG_SCANCODE_Y = 28,
    OMG_SCANCODE_Z = 29,
    OMG_SCANCODE_1 = 30,
    OMG_SCANCODE_2 = 31,
    OMG_SCANCODE_3 = 32,
    OMG_SCANCODE_4 = 33,
    OMG_SCANCODE_5 = 34,
    OMG_SCANCODE_6 = 35,
    OMG_SCANCODE_7 = 36,
    OMG_SCANCODE_8 = 37,
    OMG_SCANCODE_9 = 38,
    OMG_SCANCODE_0 = 39,
    OMG_SCANCODE_RETURN = 40,
    OMG_SCANCODE_ESCAPE = 41,
    OMG_SCANCODE_BACKSPACE = 42,
    OMG_SCANCODE_TAB = 43,
    OMG_SCANCODE_SPACE = 44,
    OMG_SCANCODE_MINUS = 45,
    OMG_SCANCODE_EQUALS = 46,
    OMG_SCANCODE_LEFTBRACKET = 47,
    OMG_SCANCODE_RIGHTBRACKET = 48,
    OMG_SCANCODE_BACKSLASH = 49,
    OMG_SCANCODE_NONUSHASH = 50,
    OMG_SCANCODE_SEMICOLON = 51,
    OMG_SCANCODE_APOSTROPHE = 52,
    OMG_SCANCODE_GRAVE = 53,
    OMG_SCANCODE_COMMA = 54,
    OMG_SCANCODE_PERIOD = 55,
    OMG_SCANCODE_SLASH = 56,
    OMG_SCANCODE_CAPSLOCK = 57,
    OMG_SCANCODE_F1 = 58,
    OMG_SCANCODE_F2 = 59,
    OMG_SCANCODE_F3 = 60,
    OMG_SCANCODE_F4 = 61,
    OMG_SCANCODE_F5 = 62,
    OMG_SCANCODE_F6 = 63,
    OMG_SCANCODE_F7 = 64,
    OMG_SCANCODE_F8 = 65,
    OMG_SCANCODE_F9 = 66,
    OMG_SCANCODE_F10 = 67,
    OMG_SCANCODE_F11 = 68,
    OMG_SCANCODE_F12 = 69,
    OMG_SCANCODE_PRINTSCREEN = 70,
    OMG_SCANCODE_SCROLLLOCK = 71,
    OMG_SCANCODE_PAUSE = 72,
    OMG_SCANCODE_INSERT = 73,
    OMG_SCANCODE_HOME = 74,
    OMG_SCANCODE_PAGEUP = 75,
    OMG_SCANCODE_DELETE = 76,
    OMG_SCANCODE_END = 77,
    OMG_SCANCODE_PAGEDOWN = 78,
    OMG_SCANCODE_RIGHT = 79,
    OMG_SCANCODE_LEFT = 80,
    OMG_SCANCODE_DOWN = 81,
    OMG_SCANCODE_UP = 82,
    OMG_SCANCODE_NUMLOCKCLEAR = 83,
    OMG_SCANCODE_KP_DIVIDE = 84,
    OMG_SCANCODE_KP_MULTIPLY = 85,
    OMG_SCANCODE_KP_MINUS = 86,
    OMG_SCANCODE_KP_PLUS = 87,
    OMG_SCANCODE_KP_ENTER = 88,
    OMG_SCANCODE_KP_1 = 89,
    OMG_SCANCODE_KP_2 = 90,
    OMG_SCANCODE_KP_3 = 91,
    OMG_SCANCODE_KP_4 = 92,
    OMG_SCANCODE_KP_5 = 93,
    OMG_SCANCODE_KP_6 = 94,
    OMG_SCANCODE_KP_7 = 95,
    OMG_SCANCODE_KP_8 = 96,
    OMG_SCANCODE_KP_9 = 97,
    OMG_SCANCODE_KP_0 = 98,
    OMG_SCANCODE_KP_PERIOD = 99,
    OMG_SCANCODE_NONUSBACKSLASH = 100,
    OMG_SCANCODE_APPLICATION = 101,
    OMG_SCANCODE_POWER = 102,
    OMG_SCANCODE_KP_EQUALS = 103,
    OMG_SCANCODE_F13 = 104,
    OMG_SCANCODE_F14 = 105,
    OMG_SCANCODE_F15 = 106,
    OMG_SCANCODE_F16 = 107,
    OMG_SCANCODE_F17 = 108,
    OMG_SCANCODE_F18 = 109,
    OMG_SCANCODE_F19 = 110,
    OMG_SCANCODE_F20 = 111,
    OMG_SCANCODE_F21 = 112,
    OMG_SCANCODE_F22 = 113,
    OMG_SCANCODE_F23 = 114,
    OMG_SCANCODE_F24 = 115,
    OMG_SCANCODE_EXECUTE = 116,
    OMG_SCANCODE_HELP = 117,
    OMG_SCANCODE_MENU = 118,
    OMG_SCANCODE_SELECT = 119,
    OMG_SCANCODE_STOP = 120,
    OMG_SCANCODE_AGAIN = 121,
    OMG_SCANCODE_UNDO = 122,
    OMG_SCANCODE_CUT = 123,
    OMG_SCANCODE_COPY = 124,
    OMG_SCANCODE_PASTE = 125,
    OMG_SCANCODE_FIND = 126,
    OMG_SCANCODE_MUTE = 127,
    OMG_SCANCODE_VOLUMEUP = 128,
    OMG_SCANCODE_VOLUMEDOWN = 129,
    OMG_SCANCODE_KP_COMMA = 133,
    OMG_SCANCODE_KP_EQUALSAS400 = 134,
    OMG_SCANCODE_INTERNATIONAL1 = 135,
    OMG_SCANCODE_INTERNATIONAL2 = 136,
    OMG_SCANCODE_INTERNATIONAL3 = 137,
    OMG_SCANCODE_INTERNATIONAL4 = 138,
    OMG_SCANCODE_INTERNATIONAL5 = 139,
    OMG_SCANCODE_INTERNATIONAL6 = 140,
    OMG_SCANCODE_INTERNATIONAL7 = 141,
    OMG_SCANCODE_INTERNATIONAL8 = 142,
    OMG_SCANCODE_INTERNATIONAL9 = 143,
    OMG_SCANCODE_LANG1 = 144,
    OMG_SCANCODE_LANG2 = 145,
    OMG_SCANCODE_LANG3 = 146,
    OMG_SCANCODE_LANG4 = 147,
    OMG_SCANCODE_LANG5 = 148,
    OMG_SCANCODE_LANG6 = 149,
    OMG_SCANCODE_LANG7 = 150,
    OMG_SCANCODE_LANG8 = 151,
    OMG_SCANCODE_LANG9 = 152,
    OMG_SCANCODE_ALTERASE = 153,
    OMG_SCANCODE_SYSREQ = 154,
    OMG_SCANCODE_CANCEL = 155,
    OMG_SCANCODE_CLEAR = 156,
    OMG_SCANCODE_PRIOR = 157,
    OMG_SCANCODE_RETURN2 = 158,
    OMG_SCANCODE_SEPARATOR = 159,
    OMG_SCANCODE_OUT = 160,
    OMG_SCANCODE_OPER = 161,
    OMG_SCANCODE_CLEARAGAIN = 162,
    OMG_SCANCODE_CRSEL = 163,
    OMG_SCANCODE_EXSEL = 164,
    OMG_SCANCODE_KP_00 = 176,
    OMG_SCANCODE_KP_000 = 177,
    OMG_SCANCODE_THOUSANDSSEPARATOR = 178,
    OMG_SCANCODE_DECIMALSEPARATOR = 179,
    OMG_SCANCODE_CURRENCYUNIT = 180,
    OMG_SCANCODE_CURRENCYSUBUNIT = 181,
    OMG_SCANCODE_KP_LEFTPAREN = 182,
    OMG_SCANCODE_KP_RIGHTPAREN = 183,
    OMG_SCANCODE_KP_LEFTBRACE = 184,
    OMG_SCANCODE_KP_RIGHTBRACE = 185,
    OMG_SCANCODE_KP_TAB = 186,
    OMG_SCANCODE_KP_BACKSPACE = 187,
    OMG_SCANCODE_KP_A = 188,
    OMG_SCANCODE_KP_B = 189,
    OMG_SCANCODE_KP_C = 190,
    OMG_SCANCODE_KP_D = 191,
    OMG_SCANCODE_KP_E = 192,
    OMG_SCANCODE_KP_F = 193,
    OMG_SCANCODE_KP_XOR = 194,
    OMG_SCANCODE_KP_POWER = 195,
    OMG_SCANCODE_KP_PERCENT = 196,
    OMG_SCANCODE_KP_LESS = 197,
    OMG_SCANCODE_KP_GREATER = 198,
    OMG_SCANCODE_KP_AMPERSAND = 199,
    OMG_SCANCODE_KP_DBLAMPERSAND = 200,
    OMG_SCANCODE_KP_VERTICALBAR = 201,
    OMG_SCANCODE_KP_DBLVERTICALBAR = 202,
    OMG_SCANCODE_KP_COLON = 203,
    OMG_SCANCODE_KP_HASH = 204,
    OMG_SCANCODE_KP_SPACE = 205,
    OMG_SCANCODE_KP_AT = 206,
    OMG_SCANCODE_KP_EXCLAM = 207,
    OMG_SCANCODE_KP_MEMSTORE = 208,
    OMG_SCANCODE_KP_MEMRECALL = 209,
    OMG_SCANCODE_KP_MEMCLEAR = 210,
    OMG_SCANCODE_KP_MEMADD = 211,
    OMG_SCANCODE_KP_MEMSUBTRACT = 212,
    OMG_SCANCODE_KP_MEMMULTIPLY = 213,
    OMG_SCANCODE_KP_MEMDIVIDE = 214,
    OMG_SCANCODE_KP_PLUSMINUS = 215,
    OMG_SCANCODE_KP_CLEAR = 216,
    OMG_SCANCODE_KP_CLEARENTRY = 217,
    OMG_SCANCODE_KP_BINARY = 218,
    OMG_SCANCODE_KP_OCTAL = 219,
    OMG_SCANCODE_KP_DECIMAL = 220,
    OMG_SCANCODE_KP_HEXADECIMAL = 221,
    OMG_SCANCODE_LCTRL = 224,
    OMG_SCANCODE_LSHIFT = 225,
    OMG_SCANCODE_LALT = 226,
    OMG_SCANCODE_LGUI = 227,
    OMG_SCANCODE_RCTRL = 228,
    OMG_SCANCODE_RSHIFT = 229,
    OMG_SCANCODE_RALT = 230,
    OMG_SCANCODE_RGUI = 231,
    OMG_SCANCODE_MODE = 257,
    OMG_SCANCODE_AUDIONEXT = 258,
    OMG_SCANCODE_AUDIOPREV = 259,
    OMG_SCANCODE_AUDIOSTOP = 260,
    OMG_SCANCODE_AUDIOPLAY = 261,
    OMG_SCANCODE_AUDIOMUTE = 262,
    OMG_SCANCODE_MEDIASELECT = 263,
    OMG_SCANCODE_WWW = 264,
    OMG_SCANCODE_MAIL = 265,
    OMG_SCANCODE_CALCULATOR = 266,
    OMG_SCANCODE_COMPUTER = 267,
    OMG_SCANCODE_AC_SEARCH = 268,
    OMG_SCANCODE_AC_HOME = 269,
    OMG_SCANCODE_AC_BACK = 270,
    OMG_SCANCODE_AC_FORWARD = 271,
    OMG_SCANCODE_AC_STOP = 272,
    OMG_SCANCODE_AC_REFRESH = 273,
    OMG_SCANCODE_AC_BOOKMARKS = 274,
    OMG_SCANCODE_BRIGHTNESSDOWN = 275,
    OMG_SCANCODE_BRIGHTNESSUP = 276,
    OMG_SCANCODE_DISPLAYSWITCH = 277,
    OMG_SCANCODE_KBDILLUMTOGGLE = 278,
    OMG_SCANCODE_KBDILLUMDOWN = 279,
    OMG_SCANCODE_KBDILLUMUP = 280,
    OMG_SCANCODE_EJECT = 281,
    OMG_SCANCODE_SLEEP = 282,
    OMG_SCANCODE_APP1 = 283,
    OMG_SCANCODE_APP2 = 284,
    OMG_SCANCODE_AUDIOREWIND = 285,
    OMG_SCANCODE_AUDIOFASTFORWARD = 286,
    OMG_SCANCODE_SOFTLEFT = 287,
    OMG_SCANCODE_SOFTRIGHT = 288,
    OMG_SCANCODE_CALL = 289,
    OMG_SCANCODE_ENDCALL = 290,
    OMG_NUM_SCANCODES = 512
} OMG_Scancode;

typedef enum
{
    OMGK_UNKNOWN = 0,
    OMGK_RETURN = '\r',
    OMGK_ESCAPE = '\x1B',
    OMGK_BACKSPACE = '\b',
    OMGK_TAB = '\t',
    OMGK_SPACE = ' ',
    OMGK_EXCLAIM = '!',
    OMGK_QUOTEDBL = '"',
    OMGK_HASH = '#',
    OMGK_PERCENT = '%',
    OMGK_DOLLAR = '$',
    OMGK_AMPERSAND = '&',
    OMGK_QUOTE = '\'',
    OMGK_LEFTPAREN = '(',
    OMGK_RIGHTPAREN = ')',
    OMGK_ASTERISK = '*',
    OMGK_PLUS = '+',
    OMGK_COMMA = ',',
    OMGK_MINUS = '-',
    OMGK_PERIOD = '.',
    OMGK_SLASH = '/',
    OMGK_0 = '0',
    OMGK_1 = '1',
    OMGK_2 = '2',
    OMGK_3 = '3',
    OMGK_4 = '4',
    OMGK_5 = '5',
    OMGK_6 = '6',
    OMGK_7 = '7',
    OMGK_8 = '8',
    OMGK_9 = '9',
    OMGK_COLON = ':',
    OMGK_SEMICOLON = ';',
    OMGK_LESS = '<',
    OMGK_EQUALS = '=',
    OMGK_GREATER = '>',
    OMGK_QUESTION = '?',
    OMGK_AT = '@',
    OMGK_LEFTBRACKET = '[',
    OMGK_BACKSLASH = '\\',
    OMGK_RIGHTBRACKET = ']',
    OMGK_CARET = '^',
    OMGK_UNDERSCORE = '_',
    OMGK_BACKQUOTE = '`',
    OMGK_a = 'a',
    OMGK_b = 'b',
    OMGK_c = 'c',
    OMGK_d = 'd',
    OMGK_e = 'e',
    OMGK_f = 'f',
    OMGK_g = 'g',
    OMGK_h = 'h',
    OMGK_i = 'i',
    OMGK_j = 'j',
    OMGK_k = 'k',
    OMGK_l = 'l',
    OMGK_m = 'm',
    OMGK_n = 'n',
    OMGK_o = 'o',
    OMGK_p = 'p',
    OMGK_q = 'q',
    OMGK_r = 'r',
    OMGK_s = 's',
    OMGK_t = 't',
    OMGK_u = 'u',
    OMGK_v = 'v',
    OMGK_w = 'w',
    OMGK_x = 'x',
    OMGK_y = 'y',
    OMGK_z = 'z',
    OMGK_CAPSLOCK = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_CAPSLOCK),
    OMGK_F1 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F1),
    OMGK_F2 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F2),
    OMGK_F3 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F3),
    OMGK_F4 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F4),
    OMGK_F5 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F5),
    OMGK_F6 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F6),
    OMGK_F7 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F7),
    OMGK_F8 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F8),
    OMGK_F9 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F9),
    OMGK_F10 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F10),
    OMGK_F11 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F11),
    OMGK_F12 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F12),
    OMGK_PRINTSCREEN = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_PRINTSCREEN),
    OMGK_SCROLLLOCK = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_SCROLLLOCK),
    OMGK_PAUSE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_PAUSE),
    OMGK_INSERT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_INSERT),
    OMGK_HOME = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_HOME),
    OMGK_PAGEUP = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_PAGEUP),
    OMGK_DELETE = '\x7F',
    OMGK_END = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_END),
    OMGK_PAGEDOWN = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_PAGEDOWN),
    OMGK_RIGHT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_RIGHT),
    OMGK_LEFT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_LEFT),
    OMGK_DOWN = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_DOWN),
    OMGK_UP = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_UP),
    OMGK_NUMLOCKCLEAR = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_NUMLOCKCLEAR),
    OMGK_KP_DIVIDE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_DIVIDE),
    OMGK_KP_MULTIPLY = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_MULTIPLY),
    OMGK_KP_MINUS = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_MINUS),
    OMGK_KP_PLUS = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_PLUS),
    OMGK_KP_ENTER = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_ENTER),
    OMGK_KP_1 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_1),
    OMGK_KP_2 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_2),
    OMGK_KP_3 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_3),
    OMGK_KP_4 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_4),
    OMGK_KP_5 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_5),
    OMGK_KP_6 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_6),
    OMGK_KP_7 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_7),
    OMGK_KP_8 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_8),
    OMGK_KP_9 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_9),
    OMGK_KP_0 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_0),
    OMGK_KP_PERIOD = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_PERIOD),
    OMGK_APPLICATION = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_APPLICATION),
    OMGK_POWER = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_POWER),
    OMGK_KP_EQUALS = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_EQUALS),
    OMGK_F13 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F13),
    OMGK_F14 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F14),
    OMGK_F15 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F15),
    OMGK_F16 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F16),
    OMGK_F17 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F17),
    OMGK_F18 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F18),
    OMGK_F19 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F19),
    OMGK_F20 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F20),
    OMGK_F21 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F21),
    OMGK_F22 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F22),
    OMGK_F23 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F23),
    OMGK_F24 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_F24),
    OMGK_EXECUTE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_EXECUTE),
    OMGK_HELP = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_HELP),
    OMGK_MENU = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_MENU),
    OMGK_SELECT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_SELECT),
    OMGK_STOP = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_STOP),
    OMGK_AGAIN = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AGAIN),
    OMGK_UNDO = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_UNDO),
    OMGK_CUT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_CUT),
    OMGK_COPY = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_COPY),
    OMGK_PASTE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_PASTE),
    OMGK_FIND = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_FIND),
    OMGK_MUTE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_MUTE),
    OMGK_VOLUMEUP = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_VOLUMEUP),
    OMGK_VOLUMEDOWN = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_VOLUMEDOWN),
    OMGK_KP_COMMA = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_COMMA),
    OMGK_KP_EQUALSAS400 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_EQUALSAS400),
    OMGK_ALTERASE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_ALTERASE),
    OMGK_SYSREQ = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_SYSREQ),
    OMGK_CANCEL = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_CANCEL),
    OMGK_CLEAR = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_CLEAR),
    OMGK_PRIOR = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_PRIOR),
    OMGK_RETURN2 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_RETURN2),
    OMGK_SEPARATOR = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_SEPARATOR),
    OMGK_OUT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_OUT),
    OMGK_OPER = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_OPER),
    OMGK_CLEARAGAIN = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_CLEARAGAIN),
    OMGK_CRSEL = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_CRSEL),
    OMGK_EXSEL = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_EXSEL),
    OMGK_KP_00 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_00),
    OMGK_KP_000 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_000),
    OMGK_THOUSANDSSEPARATOR = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_THOUSANDSSEPARATOR),
    OMGK_DECIMALSEPARATOR = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_DECIMALSEPARATOR),
    OMGK_CURRENCYUNIT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_CURRENCYUNIT),
    OMGK_CURRENCYSUBUNIT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_CURRENCYSUBUNIT),
    OMGK_KP_LEFTPAREN = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_LEFTPAREN),
    OMGK_KP_RIGHTPAREN = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_RIGHTPAREN),
    OMGK_KP_LEFTBRACE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_LEFTBRACE),
    OMGK_KP_RIGHTBRACE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_RIGHTBRACE),
    OMGK_KP_TAB = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_TAB),
    OMGK_KP_BACKSPACE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_BACKSPACE),
    OMGK_KP_A = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_A),
    OMGK_KP_B = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_B),
    OMGK_KP_C = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_C),
    OMGK_KP_D = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_D),
    OMGK_KP_E = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_E),
    OMGK_KP_F = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_F),
    OMGK_KP_XOR = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_XOR),
    OMGK_KP_POWER = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_POWER),
    OMGK_KP_PERCENT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_PERCENT),
    OMGK_KP_LESS = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_LESS),
    OMGK_KP_GREATER = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_GREATER),
    OMGK_KP_AMPERSAND = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_AMPERSAND),
    OMGK_KP_DBLAMPERSAND = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_DBLAMPERSAND),
    OMGK_KP_VERTICALBAR = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_VERTICALBAR),
    OMGK_KP_DBLVERTICALBAR = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_DBLVERTICALBAR),
    OMGK_KP_COLON = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_COLON),
    OMGK_KP_HASH = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_HASH),
    OMGK_KP_SPACE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_SPACE),
    OMGK_KP_AT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_AT),
    OMGK_KP_EXCLAM = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_EXCLAM),
    OMGK_KP_MEMSTORE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_MEMSTORE),
    OMGK_KP_MEMRECALL = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_MEMRECALL),
    OMGK_KP_MEMCLEAR = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_MEMCLEAR),
    OMGK_KP_MEMADD = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_MEMADD),
    OMGK_KP_MEMSUBTRACT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_MEMSUBTRACT),
    OMGK_KP_MEMMULTIPLY = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_MEMMULTIPLY),
    OMGK_KP_MEMDIVIDE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_MEMDIVIDE),
    OMGK_KP_PLUSMINUS = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_PLUSMINUS),
    OMGK_KP_CLEAR = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_CLEAR),
    OMGK_KP_CLEARENTRY = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_CLEARENTRY),
    OMGK_KP_BINARY = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_BINARY),
    OMGK_KP_OCTAL = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_OCTAL),
    OMGK_KP_DECIMAL = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_DECIMAL),
    OMGK_KP_HEXADECIMAL = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KP_HEXADECIMAL),
    OMGK_LCTRL = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_LCTRL),
    OMGK_LSHIFT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_LSHIFT),
    OMGK_LALT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_LALT),
    OMGK_LGUI = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_LGUI),
    OMGK_RCTRL = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_RCTRL),
    OMGK_RSHIFT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_RSHIFT),
    OMGK_RALT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_RALT),
    OMGK_RGUI = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_RGUI),
    OMGK_MODE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_MODE),
    OMGK_AUDIONEXT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AUDIONEXT),
    OMGK_AUDIOPREV = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AUDIOPREV),
    OMGK_AUDIOSTOP = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AUDIOSTOP),
    OMGK_AUDIOPLAY = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AUDIOPLAY),
    OMGK_AUDIOMUTE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AUDIOMUTE),
    OMGK_MEDIASELECT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_MEDIASELECT),
    OMGK_WWW = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_WWW),
    OMGK_MAIL = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_MAIL),
    OMGK_CALCULATOR = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_CALCULATOR),
    OMGK_COMPUTER = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_COMPUTER),
    OMGK_AC_SEARCH = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AC_SEARCH),
    OMGK_AC_HOME = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AC_HOME),
    OMGK_AC_BACK = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AC_BACK),
    OMGK_AC_FORWARD = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AC_FORWARD),
    OMGK_AC_STOP = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AC_STOP),
    OMGK_AC_REFRESH = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AC_REFRESH),
    OMGK_AC_BOOKMARKS = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AC_BOOKMARKS),
    OMGK_BRIGHTNESSDOWN = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_BRIGHTNESSDOWN),
    OMGK_BRIGHTNESSUP = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_BRIGHTNESSUP),
    OMGK_DISPLAYSWITCH = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_DISPLAYSWITCH),
    OMGK_KBDILLUMTOGGLE = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KBDILLUMTOGGLE),
    OMGK_KBDILLUMDOWN = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KBDILLUMDOWN),
    OMGK_KBDILLUMUP = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_KBDILLUMUP),
    OMGK_EJECT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_EJECT),
    OMGK_SLEEP = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_SLEEP),
    OMGK_APP1 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_APP1),
    OMGK_APP2 = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_APP2),
    OMGK_AUDIOREWIND = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AUDIOREWIND),
    OMGK_AUDIOFASTFORWARD = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_AUDIOFASTFORWARD),
    OMGK_SOFTLEFT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_SOFTLEFT),
    OMGK_SOFTRIGHT = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_SOFTRIGHT),
    OMGK_CALL = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_CALL),
    OMGK_ENDCALL = OMG_SCANCODE_TO_KEYCODE(OMG_SCANCODE_ENDCALL)
} OMG_KeyCode;

typedef enum
{
    OMG_KMOD_NONE = 0x0000,
    OMG_KMOD_LSHIFT = 0x0001,
    OMG_KMOD_RSHIFT = 0x0002,
    OMG_KMOD_LCTRL = 0x0040,
    OMG_KMOD_RCTRL = 0x0080,
    OMG_KMOD_LALT = 0x0100,
    OMG_KMOD_RALT = 0x0200,
    OMG_KMOD_LGUI = 0x0400,
    OMG_KMOD_RGUI = 0x0800,
    OMG_KMOD_NUM = 0x1000,
    OMG_KMOD_CAPS = 0x2000,
    OMG_KMOD_MODE = 0x4000,
    OMG_KMOD_SCROLL = 0x8000,
    OMG_KMOD_CTRL = OMG_KMOD_LCTRL | OMG_KMOD_RCTRL,
    OMG_KMOD_SHIFT = OMG_KMOD_LSHIFT | OMG_KMOD_RSHIFT,
    OMG_KMOD_ALT = OMG_KMOD_LALT | OMG_KMOD_RALT,
    OMG_KMOD_GUI = OMG_KMOD_LGUI | OMG_KMOD_RGUI,
    OMG_KMOD_RESERVED = OMG_KMOD_SCROLL
} OMG_Keymod;

OMG_API const OMG_String* omg_keyboard_name_from_scancode(OMG_Scancode scancode);
