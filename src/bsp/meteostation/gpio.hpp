#ifndef BSP_METEOSTATION_GPIO
#define BSP_METEOSTATION_GPIO

#include <csp.hpp>

namespace bsp::gpio
{
using status = csp::gpio::Gpio<csp::gpio::Port::_C, csp::gpio::Pin::_13,
                               csp::gpio::Mode::push_pull,
                               csp::gpio::Speed::high, csp::gpio::Pullup::none>;
}

#endif /* BSP_METEOSTATION_GPIO */
