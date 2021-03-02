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

void init(Number number);
void send(Number number, uint8_t data[], std::size_t size);
}

#endif /* CSP_SHARE_USB */
