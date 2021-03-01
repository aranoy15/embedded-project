
#include <csp.hpp>
#include <cstring>

int main(void)
{
    using namespace csp::gpio;
    using led_t = Gpio<Port::_C, Pin::_13, Mode::push_pull, Speed::high, Pullup::none>;

    csp::init();
    csp::rcc::init();
    csp::usb::init(csp::usb::Number::_1, csp::usb::Speed::full_speed);
    led_t::init();

    uint8_t data[] = "Hello, World!\r\n";

    for (;;) {
        led_t::toggle();
        csp::usb::send(data, strlen((const char*)data));
        csp::tick::delay(500);
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
