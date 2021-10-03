//
// Created by Антон Федосеев on 30.08.2021.
//

#ifndef SRC_BSP_METEOSTATION_BSP_GPIO_HPP
#define SRC_BSP_METEOSTATION_BSP_GPIO_HPP

#include <bsp_gpio_template.hpp>

namespace bsp::gpio
{
using status = csp::gpio::Gpio<
    csp::gpio::Port::_C,
    csp::gpio::Pin::_13,
    csp::gpio::Mode::PushPull>;
}

#endif  // SRC_BSP_METEOSTATION_BSP_GPIO_HPP
