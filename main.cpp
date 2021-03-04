
#include <csp.hpp>
#include <cstring>

using namespace csp::gpio;
using led_t = Gpio<Port::_C, Pin::_13, Mode::push_pull, Speed::high, Pullup::none>;

namespace
{
uint8_t rx_data[255];
std::size_t rx_size = 0;
}


namespace csp::usb
{
void receive_callback(Number number, uint8_t data[], std::size_t size)
{
    if (number == csp::usb::Number::_1) {
        if (rx_size < sizeof(rx_data)) {
            memcpy(&rx_data[rx_size], data, size);

            if ((rx_size + size) > sizeof(rx_data))
                rx_size = sizeof(rx_data);
            else
                rx_size += size;
        }
    }
}
}


int main(void)
{
    constexpr auto usb_number = csp::usb::Number::_1;

    csp::init();
    csp::rcc::init();
    led_t::init();

    csp::usb::device::cdc::init(usb_number);
    csp::usb::start_receive(usb_number);

    auto start = csp::tick::current();

    for (;;) {
        if ((csp::tick::current() - start) >= 500) {
            led_t::toggle();
            start = csp::tick::current();
        }

        if (rx_size > 0) {
            csp::usb::transmit(usb_number, rx_data, rx_size);
            rx_size = 0;
        }
    }

    return 0;
}

namespace csp::rcc
{
void systick_handler()
{
    csp::rcc::increment_tick();
}
}
