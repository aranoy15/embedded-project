#ifndef CSP_SHARE_GPIO
#define CSP_SHARE_GPIO

namespace csp::gpio
{
enum class port
{
    None = 0,
    _A,
    _B,
    _C,
    _D,
    _E,
    _F,
    _G
};

enum class pin
{
    _0 = 0,
    _1,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    _10,
    _11,
    _12,
    _13,
    _14,
    _15,
    _16,
    _17,
    _18,
    _19,
    _20,
    _21,
    _22,
    _23,
    _24,
    _25,
    _26,
    _27,
    _28,
    _29,
    _30,
    _31,
};

enum class mode
{
    push_pull = 0,
    open_drain,
    input,
    analog
};

enum class speed
{
    low = 0,
    medium,
    high,
    very_high
};

enum class pullup
{
    none = 0,
    up,
    down
};

template<port _port, pin _pin, mode _mode, speed _speed, pullup _pullup>
struct GpioBase
{
    static void init() {}
    static void on() {}
    static void off() {}
    static void toggle() {}
    static bool state() { return false; }
};

}

#endif /* CSP_SHARE_GPIO */
