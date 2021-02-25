#ifndef CSP_ST_F103RB_GPIO
#define CSP_ST_F103RB_GPIO

#include <stm32f1xx_hal.h>
#include <share/csp.hpp>

namespace csp::gpio
{
using def_t = GPIO_TypeDef;

void init_clk(csp::gpio::port port);
def_t* port_to_def(csp::gpio::port port);
csp::gpio::port def_to_port(def_t* def);

    void init(const GpioInfo& info) 
    {
        GPIO_InitTypeDef config;

        switch (info.mode) {
            case csp::gpio::mode::push_pull:
                config.Mode = GPIO_MODE_OUTPUT_PP;
                break;
            case csp::gpio::mode::open_drain:
                config.Mode = GPIO_MODE_OUTPUT_OD;
                break;
            case csp::gpio::mode::input:
                config.Mode = GPIO_MODE_INPUT;
                break;
        }

        switch (info.speed) {
            default:
            case speed::low:
                config.Speed = GPIO_SPEED_FREQ_LOW;
                break;
            case speed::medium:
                config.Speed = GPIO_SPEED_FREQ_MEDIUM;
                break;
            case speed::high:
                config.Speed = GPIO_SPEED_FREQ_HIGH;
                break;
        }

        switch (_pullup) {
            default:
            case pullup::none:
                config.Pull = GPIO_NOPULL;
                break;
            case pullup::up:
                config.Pull = GPIO_PULLUP;
                break;
            case pullup::down:
                config.Pull = GPIO_PULLDOWN;
                break;
        }

        config.Pin = pin_value();

        init_clk(_port);

        def_t* port_res = port_to_def(_port);

        if (port_res == nullptr)
            csp::error_callback(__FILE__, __LINE__);

        HAL_GPIO_Init(port_res, &config);
    }

    static void on() 
    {
        pin_t pin = pin_value();
        def_t* port = port_to_def(_port);

        if (port == nullptr)
            csp::error_callback(__FILE__, __LINE__);
        
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
    }

    static void off() 
    {
        pin_t pin = pin_value();
        def_t* port = port_to_def(_port);

        if (port == nullptr)
            csp::error_callback(__FILE__, __LINE__);
        
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
    }

    static void toggle() 
    {
        pin_t pin = pin_value();
        def_t* port = port_to_def(_port);

        if (port == nullptr)
            csp::error_callback(__FILE__, __LINE__);
        
        HAL_GPIO_TogglePin(port, pin);
    }

    static bool state() 
    { 
        pin_t pin = pin_value();
        def_t* port = port_to_def(_port);

        if (port == nullptr)
            csp::error_callback(__FILE__, __LINE__);

        return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET;
    }

private:
    static pin_t pin_value()
    {
        return static_cast<pin_t>(1 << static_cast<pin_t>(_pin));
    }
};
}  // namespace csp::gpio

#endif /* CSP_ST_F103RB_GPIO */
