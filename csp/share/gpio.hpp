#ifndef CSP_SHARE_GPIO
#define CSP_SHARE_GPIO

namespace csp::gpio
{
enum class Port
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

enum class Pin
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

enum class Mode
{
    push_pull = 0,
    open_drain,
    input,
    analog
};

enum class Speed
{
    low = 0,
    medium,
    high,
    very_high
};

enum class Pullup
{
    none = 0,
    up,
    down
};

struct GpioInfo
{
    Port port;
    Pin pin;
    Mode mode;
    Speed speed;
    Pullup pullup;

    GpioInfo(Port port, Pin pin, Mode mode, Speed speed, Pullup pullup)
        : port(port), pin(pin), mode(mode), speed(speed), pullup(pullup)
    {
    }
};

void init(const GpioInfo& info);
void on(const GpioInfo& info);
void off(const GpioInfo& info);
void toggle(const GpioInfo& info);
bool state(const GpioInfo& info);
void deinit(const GpioInfo& info);

template<Port port, Pin pin, Mode mode, Speed speed, Pullup pullup>
struct Gpio
{
    static void init() { csp::gpio::init(_info); }
    static void deinit() { csp::gpio::deinit(_info); }
    static void on() { csp::gpio::on(_info); }
    static void off() { csp::gpio::off(_info); }
    static void toggle() { csp::gpio::toggle(_info); }
    static bool state() { return csp::gpio::state(_info); }

private:
    inline static const GpioInfo _info = GpioInfo(port, pin, mode, speed, pullup);
};

}

#endif /* CSP_SHARE_GPIO */
