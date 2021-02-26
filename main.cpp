
#include <csp.hpp>

int main(void)
{
    using namespace csp::gpio;
    using led_t = Gpio<Port::_C, Pin::_13, Mode::push_pull, Speed::high, Pullup::none>;

    csp::init();
    csp::rcc::init();
    led_t::init();

    for (;;) {
        led_t::toggle();
        csp::tick::delay(500);
    }

    return 0;
}
