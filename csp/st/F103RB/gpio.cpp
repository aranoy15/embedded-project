#include <gpio.hpp>

namespace csp::gpio
{
    void init_clk(csp::gpio::port port)
    {
        switch (port) {
            case csp::gpio::port::_A:
                if (not __HAL_RCC_GPIOA_IS_CLK_ENABLED())
                    __HAL_RCC_GPIOA_CLK_ENABLE();
                break;
            case csp::gpio::port::_B:
                if (not __HAL_RCC_GPIOB_IS_CLK_ENABLED())
                    __HAL_RCC_GPIOB_CLK_ENABLE();
                break;
            case csp::gpio::port::_C:
                if (not __HAL_RCC_GPIOC_IS_CLK_ENABLED())
                    __HAL_RCC_GPIOC_CLK_ENABLE();
                break;
        }
    }

    def_t* port_to_def(csp::gpio::port port)
    {
        switch (port) {
            case csp::gpio::port::_A:
                return GPIOA;
            case csp::gpio::port::_B:
                return GPIOB;
            case csp::gpio::port::_C:
                return GPIOC;
            case csp::gpio::port::None:
            default:
                return nullptr;
        }
    }

    csp::gpio::port def_to_port(def_t* def)
    {
        if (def == nullptr) return csp::gpio::port::None;

        if (def == GPIOA)
            return csp::gpio::port::_A;
        else if (def == GPIOB)
            return csp::gpio::port::_B;
        else if (def == GPIOC)
            return csp::gpio::port::_C;
    }
}
