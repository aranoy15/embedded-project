
#include <csp.hpp>

int main(void)
{
    using led_t = csp::gpio::Gpio<csp::gpio::port::_C, csp::gpio::pin::_13, csp::gpio::mode::push_pull>;

    csp::init();
    csp::rcc::init();
    led_t::init();

    for (;;) {
        led_t::toggle();
        csp::tick::delay(1000);
    }

    return 0;
}
