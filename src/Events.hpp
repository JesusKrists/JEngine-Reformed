#pragma once

#include <string_view>
#include <type_traits>
#include <unordered_map>

#include "Base.hpp"
#include "Types.hpp"

namespace JE
{

enum class ScanCode
{
    UNKNOWN = 0,
    A = 4,
    B = 5,
    C = 6,
    D = 7,
    E = 8,
    F = 9,
    G = 10,
    H = 11,
    I = 12,
    J = 13,
    K = 14,
    L = 15,
    M = 16,
    N = 17,
    O = 18,
    P = 19,
    Q = 20,
    R = 21,
    S = 22,
    T = 23,
    U = 24,
    V = 25,
    W = 26,
    X = 27,
    Y = 28,
    Z = 29,

    KEY_1 = 30,
    KEY_2 = 31,
    KEY_3 = 32,
    KEY_4 = 33,
    KEY_5 = 34,
    KEY_6 = 35,
    KEY_7 = 36,
    KEY_8 = 37,
    KEY_9 = 38,
    KEY_0 = 39,

    RETURN = 40,
    ESCAPE = 41,
    BACKSPACE = 42,
    TAB = 43,
    SPACE = 44,

    MINUS = 45,
    EQUALS = 46,
    LEFTBRACKET = 47,
    RIGHTBRACKET = 48,
    BACKSLASH = 49,
    NONUSHASH = 50,
    SEMICOLON = 51,
    APOSTROPHE = 52,
    GRAVE = 53,
    COMMA = 54,
    PERIOD = 55,
    SLASH = 56,

    CAPSLOCK = 57,

    F1 = 58,
    F2 = 59,
    F3 = 60,
    F4 = 61,
    F5 = 62,
    F6 = 63,
    F7 = 64,
    F8 = 65,
    F9 = 66,
    F10 = 67,
    F11 = 68,
    F12 = 69,

    PRINTSCREEN = 70,
    SCROLLLOCK = 71,
    PAUSE = 72,
    INSERT = 73,
    HOME = 74,
    PAGEUP = 75,
    DELETE = 76,
    END = 77,
    PAGEDOWN = 78,
    RIGHT = 79,
    LEFT = 80,
    DOWN = 81,
    UP = 82,

    NUMLOCKCLEAR = 83,
    KP_DIVIDE = 84,
    KP_MULTIPLY = 85,
    KP_MINUS = 86,
    KP_PLUS = 87,
    KP_ENTER = 88,
    KP_1 = 89,
    KP_2 = 90,
    KP_3 = 91,
    KP_4 = 92,
    KP_5 = 93,
    KP_6 = 94,
    KP_7 = 95,
    KP_8 = 96,
    KP_9 = 97,
    KP_0 = 98,
    KP_PERIOD = 99,

    NONUSBACKSLASH = 100,
    APPLICATION = 101,
    POWER = 102,
    KP_EQUALS = 103,
    F13 = 104,
    F14 = 105,
    F15 = 106,
    F16 = 107,
    F17 = 108,
    F18 = 109,
    F19 = 110,
    F20 = 111,
    F21 = 112,
    F22 = 113,
    F23 = 114,
    F24 = 115,
    EXECUTE = 116,
    HELP = 117,
    MENU = 118,
    SELECT = 119,
    STOP = 120,
    AGAIN = 121,
    UNDO = 122,
    CUT = 123,
    COPY = 124,
    PASTE = 125,
    FIND = 126,
    MUTE = 127,
    VOLUMEUP = 128,
    VOLUMEDOWN = 129,
    KP_COMMA = 133,
    KP_EQUALSAS400 = 134,

    INTERNATIONAL1 = 135,
    INTERNATIONAL2 = 136,
    INTERNATIONAL3 = 137,
    INTERNATIONAL4 = 138,
    INTERNATIONAL5 = 139,
    INTERNATIONAL6 = 140,
    INTERNATIONAL7 = 141,
    INTERNATIONAL8 = 142,
    INTERNATIONAL9 = 143,
    LANG1 = 144,
    LANG2 = 145,
    LANG3 = 146,
    LANG4 = 147,
    LANG5 = 148,
    LANG6 = 149,
    LANG7 = 150,
    LANG8 = 151,
    LANG9 = 152,

    ALTERASE = 153,
    SYSREQ = 154,
    CANCEL = 155,
    CLEAR = 156,
    PRIOR = 157,
    RETURN2 = 158,
    SEPARATOR = 159,
    OUT = 160,
    OPER = 161,
    CLEARAGAIN = 162,
    CRSEL = 163,
    EXSEL = 164,

    KP_00 = 176,
    KP_000 = 177,
    THOUSANDSSEPARATOR = 178,
    DECIMALSEPARATOR = 179,
    CURRENCYUNIT = 180,
    CURRENCYSUBUNIT = 181,
    KP_LEFTPAREN = 182,
    KP_RIGHTPAREN = 183,
    KP_LEFTBRACE = 184,
    KP_RIGHTBRACE = 185,
    KP_TAB = 186,
    KP_BACKSPACE = 187,
    KP_A = 188,
    KP_B = 189,
    KP_C = 190,
    KP_D = 191,
    KP_E = 192,
    KP_F = 193,
    KP_XOR = 194,
    KP_POWER = 195,
    KP_PERCENT = 196,
    KP_LESS = 197,
    KP_GREATER = 198,
    KP_AMPERSAND = 199,
    KP_DBLAMPERSAND = 200,
    KP_VERTICALBAR = 201,
    KP_DBLVERTICALBAR = 202,
    KP_COLON = 203,
    KP_HASH = 204,
    KP_SPACE = 205,
    KP_AT = 206,
    KP_EXCLAM = 207,
    KP_MEMSTORE = 208,
    KP_MEMRECALL = 209,
    KP_MEMCLEAR = 210,
    KP_MEMADD = 211,
    KP_MEMSUBTRACT = 212,
    KP_MEMMULTIPLY = 213,
    KP_MEMDIVIDE = 214,
    KP_PLUSMINUS = 215,
    KP_CLEAR = 216,
    KP_CLEARENTRY = 217,
    KP_BINARY = 218,
    KP_OCTAL = 219,
    KP_DECIMAL = 220,
    KP_HEXADECIMAL = 221,

    LCTRL = 224,
    LSHIFT = 225,
    LALT = 226,
    LGUI = 227,
    RCTRL = 228,
    RSHIFT = 229,
    RALT = 230,
    RGUI = 231,

    MODE = 257,

    AUDIONEXT = 258,
    AUDIOPREV = 259,
    AUDIOSTOP = 260,
    AUDIOPLAY = 261,
    AUDIOMUTE = 262,
    MEDIASELECT = 263,
    WWW = 264,
    MAIL = 265,
    CALCULATOR = 266,
    COMPUTER = 267,
    AC_SEARCH = 268,
    AC_HOME = 269,
    AC_BACK = 270,
    AC_FORWARD = 271,
    AC_STOP = 272,
    AC_REFRESH = 273,
    AC_BOOKMARKS = 274,

    BRIGHTNESSDOWN = 275,
    BRIGHTNESSUP = 276,
    DISPLAYSWITCH = 277,
    KBDILLUMTOGGLE = 278,
    KBDILLUMDOWN = 279,
    KBDILLUMUP = 280,
    EJECT = 281,
    SLEEP = 282,

    APP1 = 283,
    APP2 = 284,

    AUDIOREWIND = 285,
    AUDIOFASTFORWARD = 286,

    SOFTLEFT = 287,
    SOFTRIGHT = 288,
    CALL = 289,
    ENDCALL = 290,

    NUM_SCANCODES = 512
};

inline constexpr auto SCANCODE_MASK = 1u << 30u;
constexpr auto ScanCodeToKeyCode(ScanCode code) -> std::uint32_t
{
    return static_cast<std::uint32_t>(code) | SCANCODE_MASK;
}

enum class KeyCode
{
    UNKNOWN = 0,

    RETURN = '\r',
    ESCAPE = '\x1B',
    BACKSPACE = '\b',
    TAB = '\t',
    SPACE = ' ',
    EXCLAIM = '!',
    QUOTEDBL = '"',
    HASH = '#',
    PERCENT = '%',
    DOLLAR = '$',
    AMPERSAND = '&',
    QUOTE = '\'',
    LEFTPAREN = '(',
    RIGHTPAREN = ')',
    ASTERISK = '*',
    PLUS = '+',
    COMMA = ',',
    MINUS = '-',
    PERIOD = '.',
    SLASH = '/',
    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',
    COLON = ':',
    SEMICOLON = ';',
    LESS = '<',
    EQUALS = '=',
    GREATER = '>',
    QUESTION = '?',
    AT = '@',

    /*
       Skip uppercase letters
     */

    LEFTBRACKET = '[',
    BACKSLASH = '\\',
    RIGHTBRACKET = ']',
    CARET = '^',
    UNDERSCORE = '_',
    BACKQUOTE = '`',
    A = 'a',
    B = 'b',
    C = 'c',
    D = 'd',
    E = 'e',
    F = 'f',
    G = 'g',
    H = 'h',
    I = 'i',
    J = 'j',
    K = 'k',
    L = 'l',
    M = 'm',
    N = 'n',
    O = 'o',
    P = 'p',
    Q = 'q',
    R = 'r',
    S = 's',
    T = 't',
    U = 'u',
    V = 'v',
    W = 'w',
    X = 'x',
    Y = 'y',
    Z = 'z',

    CAPSLOCK = ScanCodeToKeyCode(ScanCode::CAPSLOCK),

    F1 = ScanCodeToKeyCode(ScanCode::F1),
    F2 = ScanCodeToKeyCode(ScanCode::F2),
    F3 = ScanCodeToKeyCode(ScanCode::F3),
    F4 = ScanCodeToKeyCode(ScanCode::F4),
    F5 = ScanCodeToKeyCode(ScanCode::F5),
    F6 = ScanCodeToKeyCode(ScanCode::F6),
    F7 = ScanCodeToKeyCode(ScanCode::F7),
    F8 = ScanCodeToKeyCode(ScanCode::F8),
    F9 = ScanCodeToKeyCode(ScanCode::F9),
    F10 = ScanCodeToKeyCode(ScanCode::F10),
    F11 = ScanCodeToKeyCode(ScanCode::F11),
    F12 = ScanCodeToKeyCode(ScanCode::F12),

    PRINTSCREEN = ScanCodeToKeyCode(ScanCode::PRINTSCREEN),
    SCROLLLOCK = ScanCodeToKeyCode(ScanCode::SCROLLLOCK),
    PAUSE = ScanCodeToKeyCode(ScanCode::PAUSE),
    INSERT = ScanCodeToKeyCode(ScanCode::INSERT),
    HOME = ScanCodeToKeyCode(ScanCode::HOME),
    PAGEUP = ScanCodeToKeyCode(ScanCode::PAGEUP),
    DELETE = '\x7F',
    END = ScanCodeToKeyCode(ScanCode::END),
    PAGEDOWN = ScanCodeToKeyCode(ScanCode::PAGEDOWN),
    RIGHT = ScanCodeToKeyCode(ScanCode::RIGHT),
    LEFT = ScanCodeToKeyCode(ScanCode::LEFT),
    DOWN = ScanCodeToKeyCode(ScanCode::DOWN),
    UP = ScanCodeToKeyCode(ScanCode::UP),

    NUMLOCKCLEAR = ScanCodeToKeyCode(ScanCode::NUMLOCKCLEAR),
    KP_DIVIDE = ScanCodeToKeyCode(ScanCode::KP_DIVIDE),
    KP_MULTIPLY = ScanCodeToKeyCode(ScanCode::KP_MULTIPLY),
    KP_MINUS = ScanCodeToKeyCode(ScanCode::KP_MINUS),
    KP_PLUS = ScanCodeToKeyCode(ScanCode::KP_PLUS),
    KP_ENTER = ScanCodeToKeyCode(ScanCode::KP_ENTER),
    KP_1 = ScanCodeToKeyCode(ScanCode::KP_1),
    KP_2 = ScanCodeToKeyCode(ScanCode::KP_2),
    KP_3 = ScanCodeToKeyCode(ScanCode::KP_3),
    KP_4 = ScanCodeToKeyCode(ScanCode::KP_4),
    KP_5 = ScanCodeToKeyCode(ScanCode::KP_5),
    KP_6 = ScanCodeToKeyCode(ScanCode::KP_6),
    KP_7 = ScanCodeToKeyCode(ScanCode::KP_7),
    KP_8 = ScanCodeToKeyCode(ScanCode::KP_8),
    KP_9 = ScanCodeToKeyCode(ScanCode::KP_9),
    KP_0 = ScanCodeToKeyCode(ScanCode::KP_0),
    KP_PERIOD = ScanCodeToKeyCode(ScanCode::KP_PERIOD),

    APPLICATION = ScanCodeToKeyCode(ScanCode::APPLICATION),
    POWER = ScanCodeToKeyCode(ScanCode::POWER),
    KP_EQUALS = ScanCodeToKeyCode(ScanCode::KP_EQUALS),
    F13 = ScanCodeToKeyCode(ScanCode::F13),
    F14 = ScanCodeToKeyCode(ScanCode::F14),
    F15 = ScanCodeToKeyCode(ScanCode::F15),
    F16 = ScanCodeToKeyCode(ScanCode::F16),
    F17 = ScanCodeToKeyCode(ScanCode::F17),
    F18 = ScanCodeToKeyCode(ScanCode::F18),
    F19 = ScanCodeToKeyCode(ScanCode::F19),
    F20 = ScanCodeToKeyCode(ScanCode::F20),
    F21 = ScanCodeToKeyCode(ScanCode::F21),
    F22 = ScanCodeToKeyCode(ScanCode::F22),
    F23 = ScanCodeToKeyCode(ScanCode::F23),
    F24 = ScanCodeToKeyCode(ScanCode::F24),
    EXECUTE = ScanCodeToKeyCode(ScanCode::EXECUTE),
    HELP = ScanCodeToKeyCode(ScanCode::HELP),
    MENU = ScanCodeToKeyCode(ScanCode::MENU),
    SELECT = ScanCodeToKeyCode(ScanCode::SELECT),
    STOP = ScanCodeToKeyCode(ScanCode::STOP),
    AGAIN = ScanCodeToKeyCode(ScanCode::AGAIN),
    UNDO = ScanCodeToKeyCode(ScanCode::UNDO),
    CUT = ScanCodeToKeyCode(ScanCode::CUT),
    COPY = ScanCodeToKeyCode(ScanCode::COPY),
    PASTE = ScanCodeToKeyCode(ScanCode::PASTE),
    FIND = ScanCodeToKeyCode(ScanCode::FIND),
    MUTE = ScanCodeToKeyCode(ScanCode::MUTE),
    VOLUMEUP = ScanCodeToKeyCode(ScanCode::VOLUMEUP),
    VOLUMEDOWN = ScanCodeToKeyCode(ScanCode::VOLUMEDOWN),
    KP_COMMA = ScanCodeToKeyCode(ScanCode::KP_COMMA),
    KP_EQUALSAS400 = ScanCodeToKeyCode(ScanCode::KP_EQUALSAS400),

    ALTERASE = ScanCodeToKeyCode(ScanCode::ALTERASE),
    SYSREQ = ScanCodeToKeyCode(ScanCode::SYSREQ),
    CANCEL = ScanCodeToKeyCode(ScanCode::CANCEL),
    CLEAR = ScanCodeToKeyCode(ScanCode::CLEAR),
    PRIOR = ScanCodeToKeyCode(ScanCode::PRIOR),
    RETURN2 = ScanCodeToKeyCode(ScanCode::RETURN2),
    SEPARATOR = ScanCodeToKeyCode(ScanCode::SEPARATOR),
    OUT = ScanCodeToKeyCode(ScanCode::OUT),
    OPER = ScanCodeToKeyCode(ScanCode::OPER),
    CLEARAGAIN = ScanCodeToKeyCode(ScanCode::CLEARAGAIN),
    CRSEL = ScanCodeToKeyCode(ScanCode::CRSEL),
    EXSEL = ScanCodeToKeyCode(ScanCode::EXSEL),

    KP_00 = ScanCodeToKeyCode(ScanCode::KP_00),
    KP_000 = ScanCodeToKeyCode(ScanCode::KP_000),
    THOUSANDSSEPARATOR = ScanCodeToKeyCode(ScanCode::THOUSANDSSEPARATOR),
    DECIMALSEPARATOR = ScanCodeToKeyCode(ScanCode::DECIMALSEPARATOR),
    CURRENCYUNIT = ScanCodeToKeyCode(ScanCode::CURRENCYUNIT),
    CURRENCYSUBUNIT = ScanCodeToKeyCode(ScanCode::CURRENCYSUBUNIT),
    KP_LEFTPAREN = ScanCodeToKeyCode(ScanCode::KP_LEFTPAREN),
    KP_RIGHTPAREN = ScanCodeToKeyCode(ScanCode::KP_RIGHTPAREN),
    KP_LEFTBRACE = ScanCodeToKeyCode(ScanCode::KP_LEFTBRACE),
    KP_RIGHTBRACE = ScanCodeToKeyCode(ScanCode::KP_RIGHTBRACE),
    KP_TAB = ScanCodeToKeyCode(ScanCode::KP_TAB),
    KP_BACKSPACE = ScanCodeToKeyCode(ScanCode::KP_BACKSPACE),
    KP_A = ScanCodeToKeyCode(ScanCode::KP_A),
    KP_B = ScanCodeToKeyCode(ScanCode::KP_B),
    KP_C = ScanCodeToKeyCode(ScanCode::KP_C),
    KP_D = ScanCodeToKeyCode(ScanCode::KP_D),
    KP_E = ScanCodeToKeyCode(ScanCode::KP_E),
    KP_F = ScanCodeToKeyCode(ScanCode::KP_F),
    KP_XOR = ScanCodeToKeyCode(ScanCode::KP_XOR),
    KP_POWER = ScanCodeToKeyCode(ScanCode::KP_POWER),
    KP_PERCENT = ScanCodeToKeyCode(ScanCode::KP_PERCENT),
    KP_LESS = ScanCodeToKeyCode(ScanCode::KP_LESS),
    KP_GREATER = ScanCodeToKeyCode(ScanCode::KP_GREATER),
    KP_AMPERSAND = ScanCodeToKeyCode(ScanCode::KP_AMPERSAND),
    KP_DBLAMPERSAND = ScanCodeToKeyCode(ScanCode::KP_DBLAMPERSAND),
    KP_VERTICALBAR = ScanCodeToKeyCode(ScanCode::KP_VERTICALBAR),
    KP_DBLVERTICALBAR = ScanCodeToKeyCode(ScanCode::KP_DBLVERTICALBAR),
    KP_COLON = ScanCodeToKeyCode(ScanCode::KP_COLON),
    KP_HASH = ScanCodeToKeyCode(ScanCode::KP_HASH),
    KP_SPACE = ScanCodeToKeyCode(ScanCode::KP_SPACE),
    KP_AT = ScanCodeToKeyCode(ScanCode::KP_AT),
    KP_EXCLAM = ScanCodeToKeyCode(ScanCode::KP_EXCLAM),
    KP_MEMSTORE = ScanCodeToKeyCode(ScanCode::KP_MEMSTORE),
    KP_MEMRECALL = ScanCodeToKeyCode(ScanCode::KP_MEMRECALL),
    KP_MEMCLEAR = ScanCodeToKeyCode(ScanCode::KP_MEMCLEAR),
    KP_MEMADD = ScanCodeToKeyCode(ScanCode::KP_MEMADD),
    KP_MEMSUBTRACT = ScanCodeToKeyCode(ScanCode::KP_MEMSUBTRACT),
    KP_MEMMULTIPLY = ScanCodeToKeyCode(ScanCode::KP_MEMMULTIPLY),
    KP_MEMDIVIDE = ScanCodeToKeyCode(ScanCode::KP_MEMDIVIDE),
    KP_PLUSMINUS = ScanCodeToKeyCode(ScanCode::KP_PLUSMINUS),
    KP_CLEAR = ScanCodeToKeyCode(ScanCode::KP_CLEAR),
    KP_CLEARENTRY = ScanCodeToKeyCode(ScanCode::KP_CLEARENTRY),
    KP_BINARY = ScanCodeToKeyCode(ScanCode::KP_BINARY),
    KP_OCTAL = ScanCodeToKeyCode(ScanCode::KP_OCTAL),
    KP_DECIMAL = ScanCodeToKeyCode(ScanCode::KP_DECIMAL),
    KP_HEXADECIMAL = ScanCodeToKeyCode(ScanCode::KP_HEXADECIMAL),

    LCTRL = ScanCodeToKeyCode(ScanCode::LCTRL),
    LSHIFT = ScanCodeToKeyCode(ScanCode::LSHIFT),
    LALT = ScanCodeToKeyCode(ScanCode::LALT),
    LGUI = ScanCodeToKeyCode(ScanCode::LGUI),
    RCTRL = ScanCodeToKeyCode(ScanCode::RCTRL),
    RSHIFT = ScanCodeToKeyCode(ScanCode::RSHIFT),
    RALT = ScanCodeToKeyCode(ScanCode::RALT),
    RGUI = ScanCodeToKeyCode(ScanCode::RGUI),

    MODE = ScanCodeToKeyCode(ScanCode::MODE),

    AUDIONEXT = ScanCodeToKeyCode(ScanCode::AUDIONEXT),
    AUDIOPREV = ScanCodeToKeyCode(ScanCode::AUDIOPREV),
    AUDIOSTOP = ScanCodeToKeyCode(ScanCode::AUDIOSTOP),
    AUDIOPLAY = ScanCodeToKeyCode(ScanCode::AUDIOPLAY),
    AUDIOMUTE = ScanCodeToKeyCode(ScanCode::AUDIOMUTE),
    MEDIASELECT = ScanCodeToKeyCode(ScanCode::MEDIASELECT),
    WWW = ScanCodeToKeyCode(ScanCode::WWW),
    MAIL = ScanCodeToKeyCode(ScanCode::MAIL),
    CALCULATOR = ScanCodeToKeyCode(ScanCode::CALCULATOR),
    COMPUTER = ScanCodeToKeyCode(ScanCode::COMPUTER),
    AC_SEARCH = ScanCodeToKeyCode(ScanCode::AC_SEARCH),
    AC_HOME = ScanCodeToKeyCode(ScanCode::AC_HOME),
    AC_BACK = ScanCodeToKeyCode(ScanCode::AC_BACK),
    AC_FORWARD = ScanCodeToKeyCode(ScanCode::AC_FORWARD),
    AC_STOP = ScanCodeToKeyCode(ScanCode::AC_STOP),
    AC_REFRESH = ScanCodeToKeyCode(ScanCode::AC_REFRESH),
    AC_BOOKMARKS = ScanCodeToKeyCode(ScanCode::AC_BOOKMARKS),

    BRIGHTNESSDOWN = ScanCodeToKeyCode(ScanCode::BRIGHTNESSDOWN),
    BRIGHTNESSUP = ScanCodeToKeyCode(ScanCode::BRIGHTNESSUP),
    DISPLAYSWITCH = ScanCodeToKeyCode(ScanCode::DISPLAYSWITCH),
    KBDILLUMTOGGLE = ScanCodeToKeyCode(ScanCode::KBDILLUMTOGGLE),
    KBDILLUMDOWN = ScanCodeToKeyCode(ScanCode::KBDILLUMDOWN),
    KBDILLUMUP = ScanCodeToKeyCode(ScanCode::KBDILLUMUP),
    EJECT = ScanCodeToKeyCode(ScanCode::EJECT),
    SLEEP = ScanCodeToKeyCode(ScanCode::SLEEP),
    APP1 = ScanCodeToKeyCode(ScanCode::APP1),
    APP2 = ScanCodeToKeyCode(ScanCode::APP2),

    AUDIOREWIND = ScanCodeToKeyCode(ScanCode::AUDIOREWIND),
    AUDIOFASTFORWARD = ScanCodeToKeyCode(ScanCode::AUDIOFASTFORWARD),

    SOFTLEFT = ScanCodeToKeyCode(ScanCode::SOFTLEFT),
    SOFTRIGHT = ScanCodeToKeyCode(ScanCode::SOFTRIGHT),
    CALL = ScanCodeToKeyCode(ScanCode::CALL),
    ENDCALL = ScanCodeToKeyCode(ScanCode::ENDCALL)
};

enum class MouseButton
{
};

class IEvent
{
  public:
    friend class EventDispatcher;

    IEvent(const IEvent& other) = delete;
    IEvent(IEvent&& other) = delete;
    auto operator=(const IEvent& other) -> IEvent& = delete;
    auto operator=(IEvent&& other) -> IEvent& = delete;

    enum class EventCategory
    {
        UNKNOWN,
        APP,
        KEYBOARD,
        MOUSE
    };

    enum class EventType
    {
        UNKNOWN,
        QUIT,
        KEY_DOWN,
        KEY_UP,
        MOUSE_MOVE
    };

    IEvent() = default;
    virtual ~IEvent() = default;

    virtual auto Category() const -> EventCategory = 0;
    virtual auto Type() const -> EventType = 0;

    inline auto Handled() const -> bool { return m_Handled; }

  private:
    inline void SetEventHandled() { m_Handled = true; }

    bool m_Handled = false;
};

constexpr auto EventTypeToCategory(IEvent::EventType type) -> IEvent::EventCategory
{
    switch (type) {
        case IEvent::EventType::QUIT:
            return IEvent::EventCategory::APP;
        case IEvent::EventType::KEY_DOWN:
        case IEvent::EventType::KEY_UP:
            return IEvent::EventCategory::KEYBOARD;
        case IEvent::EventType::MOUSE_MOVE:
            return IEvent::EventCategory::MOUSE;

        case IEvent::EventType::UNKNOWN:
        default:
            return IEvent::EventCategory::UNKNOWN;
    }
}

constexpr auto ToString(IEvent::EventCategory category) -> std::string_view
{
    switch (category) {
        case IEvent::EventCategory::APP:
            return "APP";
        case IEvent::EventCategory::KEYBOARD:
            return "KEYBOARD";
        case IEvent::EventCategory::MOUSE:
            return "MOUSE";

        default:
            return "UNKNOWN";
    }
}

constexpr auto ToString(IEvent::EventType type) -> std::string_view
{
    switch (type) {
        case IEvent::EventType::QUIT:
            return "QUIT";
        case IEvent::EventType::KEY_DOWN:
            return "KEY_DOWN";
        case IEvent::EventType::KEY_UP:
            return "KEY_UP";
        case IEvent::EventType::MOUSE_MOVE:
            return "MOUSE_MOTION";

        default:
            return "UNKNOWN";
    }
}

class IEventProcessor
{
  public:
    IEventProcessor(const IEventProcessor& other) = delete;
    IEventProcessor(IEventProcessor&& other) = delete;
    auto operator=(const IEventProcessor& other) -> IEventProcessor& = delete;
    auto operator=(IEventProcessor&& other) -> IEventProcessor& = delete;

    IEventProcessor() = default;
    virtual ~IEventProcessor() = default;

    virtual void ProcessEvent(IEvent& event) = 0;
};

class EventDispatcher
{
  public:
    explicit EventDispatcher(IEvent& event)
        : m_Event(event)
    {
    }

    template<typename EventType, typename Func>
    inline auto Dispatch(Func func) -> bool
    {
        static_assert(std::is_base_of_v<IEvent, EventType>, "Events have to derive from IEvent");

        if (m_Event.Handled() || EventType::StaticType() != m_Event.Type()) {
            return false;
        }

        if (func(static_cast<EventType&>(m_Event))) {
            m_Event.SetEventHandled();
        }

        return true;
    }

  private:
    IEvent& m_Event;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

class UnknownEvent final : public IEvent
{
  public:
    using IEvent::IEvent;

    inline auto Category() const -> EventCategory override { return EventCategory::UNKNOWN; }
    inline auto Type() const -> EventType override { return EventType::UNKNOWN; }

    static constexpr auto StaticType() -> EventType { return EventType::UNKNOWN; }
};

class QuitEvent final : public IEvent
{
  public:
    using IEvent::IEvent;

    inline auto Category() const -> EventCategory override { return EventCategory::APP; }
    inline auto Type() const -> EventType override { return EventType::QUIT; }

    static constexpr auto StaticType() -> EventType { return EventType::QUIT; }
};

class KeyDownEvent final : public IEvent
{
  public:
    KeyDownEvent(KeyCode keyCode, bool pressed)
        : m_KeyCode(keyCode)
        , m_Pressed(pressed)
    {
    }

    inline auto Category() const -> EventCategory override { return EventCategory::KEYBOARD; }
    inline auto Type() const -> EventType override { return EventType::KEY_DOWN; }

    static constexpr auto StaticType() -> EventType { return EventType::KEY_DOWN; }

    inline auto Code() const -> KeyCode { return m_KeyCode; }
    inline auto Pressed() const -> bool { return m_Pressed; }

  private:
    KeyCode m_KeyCode;
    bool m_Pressed;
};

class KeyUpEvent final : public IEvent
{
  public:
    KeyUpEvent(KeyCode keyCode, bool pressed)
        : m_KeyCode(keyCode)
        , m_Pressed(pressed)
    {
    }

    inline auto Category() const -> EventCategory override { return EventCategory::KEYBOARD; }
    inline auto Type() const -> EventType override { return EventType::KEY_UP; }

    static constexpr auto StaticType() -> EventType { return EventType::KEY_UP; }

    inline auto Code() const -> KeyCode { return m_KeyCode; }
    inline auto Pressed() const -> bool { return m_Pressed; }

  private:
    KeyCode m_KeyCode;
    bool m_Pressed;
};

class MouseMoveEvent final : public IEvent
{
  public:
    MouseMoveEvent(const Size2D& relativeMotion,  // NOLINT(bugprone-easily-swappable-parameters)
                   const Size2D& relativePosition)
        : m_RelativeMotion(relativeMotion)
        , m_RelativePosition(relativePosition)
    {
    }

    inline auto Category() const -> EventCategory override { return EventCategory::MOUSE; }
    inline auto Type() const -> EventType override { return EventType::MOUSE_MOVE; }

    static constexpr auto StaticType() -> EventType { return EventType::MOUSE_MOVE; }

    inline auto Motion() const -> const Size2D& { return m_RelativeMotion; }
    inline auto Position() const -> const Size2D& { return m_RelativePosition; }

  private:
    Size2D m_RelativeMotion;
    Size2D m_RelativePosition;
};

class InputController : public IEventProcessor
{
  public:
    inline void NewFrame()
    {
        m_MouseFrameMotion = {0, 0};
        m_PreviousKeyMap = m_KeyMap;
    }

    inline void ProcessEvent(IEvent& event) override
    {
        EventDispatcher dispatcher{event};
        dispatcher.Dispatch<KeyDownEvent>(
            [this](const KeyDownEvent& evnt)
            {
                m_KeyMap[evnt.Code()] = evnt.Pressed();
                return true;
            });
        dispatcher.Dispatch<KeyUpEvent>(
            [this](const KeyUpEvent& evnt)
            {
                m_KeyMap[evnt.Code()] = evnt.Pressed();
                return true;
            });

        dispatcher.Dispatch<MouseMoveEvent>(
            [this](const MouseMoveEvent& evnt)
            {
                m_MouseFrameMotion.x += evnt.Motion().x;
                m_MouseFrameMotion.y += evnt.Motion().y;

                m_MousePosition = evnt.Position();

                return true;
            });
    }

    inline auto KeyPressed(KeyCode key) const -> bool
    {
        auto iter = m_KeyMap.find(key);
        if (iter != std::end(m_KeyMap)) {
            return iter->second;
        }

        return false;
    }

    inline auto MousePos() const -> const Size2D& { return m_MousePosition; }
    inline auto MouseFrameMotion() const -> const Size2D& { return m_MouseFrameMotion; }

  private:
    std::unordered_map<KeyCode, bool> m_KeyMap;
    std::unordered_map<KeyCode, bool> m_PreviousKeyMap;

    Size2D m_MouseFrameMotion;
    Size2D m_MousePosition;
};

}  // namespace JE
