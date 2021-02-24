#ifndef CSP_SHARE_UART
#define CSP_SHARE_UART

namespace csp::uart
{
enum class number
{
    _1 = 0,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8
};

enum class interface
{
    rs232 = 0,
    rs485
};

enum class parity
{
    none = 0,
    even,
    odd
};

enum class data_bits
{
    _7 = 0,
    _8,
    _9
};

enum class stop_bits
{
    _1 = 0,
    _2
};

enum class baud_rate
{
    _110 = 110,
    _300 = 300,
    _600 = 600,
    _1200 = 1200,
    _2400 = 2400,
    _4800 = 4800,
    _9600 = 9600,
    _14400 = 14400,
    _19200 = 19200,
    _38400 = 38400,
    _56000 = 56000,
    _57600 = 57600,
    _115200 = 115200,
    _128000 = 128000,
    _153600 = 153600,
    _230400 = 230400,
    _256000 = 256000,
    _460800 = 460800,
    _921600 = 921600,
};

}

#endif /* CSP_SHARE_UART */
