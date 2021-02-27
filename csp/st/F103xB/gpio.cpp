#include <csp.hpp>
#include <hal.hpp>

using namespace csp::gpio;

namespace
{
using pin_t = uint16_t;

pin_t pin_value(const GpioInfo& info)
{
    return static_cast<pin_t>(1 << static_cast<pin_t>(info.pin));
}

using def_t = GPIO_TypeDef;


}

namespace csp::gpio
{
void init_clk(Port port);
void deinit_clk(Port port);
def_t* port_to_def(Port port);
Port def_to_port(def_t* def);
uint32_t speed_parse(Speed speed);

void init_clk(csp::gpio::Port port)
{
    switch (port) {
        case Port::_A:
            if (not __HAL_RCC_GPIOA_IS_CLK_ENABLED())
                __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case Port::_B:
            if (not __HAL_RCC_GPIOB_IS_CLK_ENABLED())
                __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case Port::_C:
            if (not __HAL_RCC_GPIOC_IS_CLK_ENABLED())
                __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        case Port::_D:
            if (not __HAL_RCC_GPIOD_IS_CLK_ENABLED())
                __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
        default:
        case Port::_E:
        case Port::_F:
        case Port::_G:
            break;
    }
}

void deinit_clk(Port port)
{
    switch (port) {
        case Port::_A:
            if (__HAL_RCC_GPIOA_IS_CLK_ENABLED())
                __HAL_RCC_GPIOA_CLK_DISABLE();
            break;
        case Port::_B:
            if (__HAL_RCC_GPIOB_IS_CLK_ENABLED())
                __HAL_RCC_GPIOB_CLK_DISABLE();
            break;
        case Port::_C:
            if (__HAL_RCC_GPIOC_IS_CLK_ENABLED())
                __HAL_RCC_GPIOC_CLK_DISABLE();
            break;
        case Port::_D:
            if (__HAL_RCC_GPIOD_IS_CLK_ENABLED())
                __HAL_RCC_GPIOD_CLK_DISABLE();
            break;
        default:
        case Port::_E:
        case Port::_F:
        case Port::_G:
            break;
    }
}

def_t* port_to_def(Port port)
{
    switch (port) {
        case Port::_A:
            return GPIOA;
        case Port::_B:
            return GPIOB;
        case Port::_C:
            return GPIOC;
        case Port::None:
        default:
            return nullptr;
    }
}

Port def_to_port(def_t* def)
{
    if (def == nullptr) return Port::None;

    if (def == GPIOA)
        return Port::_A;
    else if (def == GPIOB)
        return Port::_B;
    else if (def == GPIOC)
        return Port::_C;
    else if (def == GPIOD)
        return Port::_G;
    else 
        return Port::None;
}

uint32_t speed_parse(Speed speed)
{
    switch (speed) {
        default:
        case Speed::low:
            return GPIO_SPEED_FREQ_LOW;
        case Speed::medium:
            return GPIO_SPEED_FREQ_MEDIUM;
        case Speed::very_high:
        case Speed::high:
            return GPIO_SPEED_FREQ_HIGH;
    }
}

}  // namespace csp::gpio
