#ifndef CSP_SHARE_USB
#define CSP_SHARE_USB

namespace csp::usb
{
enum class Number
{
    _1 = 0,
    _2,
    _3
};

enum class Speed
{
    full_speed = 0,
    high_speed
};

void init(Number number, Speed speed);
void send(uint8_t data[], std::size_t size);
}

#endif /* CSP_SHARE_USB */
