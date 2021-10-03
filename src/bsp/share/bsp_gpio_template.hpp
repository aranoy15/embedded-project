//
// Created by Антон Федосеев on 30.08.2021.
//

#ifndef SRC_BSP_SHARE_BSP_GPIO_TEMPLATE_HPP
#define SRC_BSP_SHARE_BSP_GPIO_TEMPLATE_HPP

namespace bsp::gpio
{
template<typename gpio, bool invert = false>
class Relay
{
public:
    Relay() = delete;

    static void on()
    {
        if (invert) gpio::off();
        else gpio::on();
    }

    static void off()
    {
        if (invert) gpio::on();
        else gpio::off();
    }

    static bool state()
    {
        if (invert) return not gpio::state();
        else return gpio::state();
    }
};
}

#endif  // SRC_BSP_SHARE_BSP_GPIO_TEMPLATE_HPP
