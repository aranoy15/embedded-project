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
    _G,
    _H,
    _I,
    _J
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
    PushPull = 0,
    OpenDrain,
    Input,
    Analog,
    AlternateOpenDrain,
    AlternatePushPull
};

enum class AlternateFunction
{
    None = 0,
    Uart1,
    Uart2,
    Uart3,
    I2c1,
    I2c2,
    I2c3,
    Spi1,
    Spi2,
    Spi3
};

enum class Speed
{
    Low = 0,
    Medium,
    High,
    VeryHigh
};

enum class Pullup
{
    None = 0,
    Up,
    Down
};

struct GpioInfo
{
    Port port;
    Pin pin;
    Mode mode;
    Speed speed;
    Pullup pullup;
    AlternateFunction alternate;

    GpioInfo(Port port, Pin pin, Mode mode, Speed speed, Pullup pullup,
             AlternateFunction alternate) noexcept : port(port),
                                                     pin(pin),
                                                     mode(mode),
                                                     speed(speed),
                                                     pullup(pullup),
                                                     alternate(alternate)
    {
    }
};

void init(const GpioInfo& info) noexcept;
void deinit(const GpioInfo& info) noexcept;
void on(const GpioInfo& info) noexcept;
void off(const GpioInfo& info) noexcept;
void toggle(const GpioInfo& info) noexcept;
bool state(const GpioInfo& info) noexcept;

template <Port port, Pin pin, Mode mode, Speed speed = Speed::Low,
          Pullup pullup = Pullup::None,
          AlternateFunction alternate = AlternateFunction::None>
struct Gpio
{
    static void init() noexcept { csp::gpio::init(_info); }
    static void deinit() noexcept { csp::gpio::deinit(_info); }

    static void on() noexcept
    {
        static_assert(mode == Mode::PushPull or mode == Mode::OpenDrain);
        csp::gpio::on(_info);
    }

    static void off() noexcept
    {
        static_assert(mode == Mode::PushPull or mode == Mode::OpenDrain);
        csp::gpio::off(_info);
    }

    static void toggle() noexcept
    {
        static_assert(mode == Mode::PushPull or mode == Mode::OpenDrain);
        csp::gpio::toggle(_info);
    }

    static bool state() noexcept { return csp::gpio::state(_info); }

private:
    inline static GpioInfo _info = GpioInfo(port, pin, mode, speed, pullup, alternate);
};
}  // namespace csp::gpio

#endif /* CSP_SHARE_GPIO */
