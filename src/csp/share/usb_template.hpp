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

enum class Status
{
    Default,
    Addressed,
    Configured,
    Suspended
};

namespace device::cdc
{
    void init(Number number);
    void deinit(Number number);
}  // namespace device::cdc

bool transmit(Number number, const uint8_t data[], std::size_t size);
void receive_callback(Number number, uint8_t data[], std::size_t size);
Status status(Number number);
}  // namespace csp::usb

#endif /* CSP_SHARE_USB */
