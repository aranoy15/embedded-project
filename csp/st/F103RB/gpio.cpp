#include <gpio.hpp>
#include <stm32f1xx_hal.h>

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

void init(const GpioInfo& info)
{
    GPIO_InitTypeDef config;

    switch (info.mode) {
        case Mode::push_pull:
            config.Mode = GPIO_MODE_OUTPUT_PP;
            break;
        case Mode::open_drain:
            config.Mode = GPIO_MODE_OUTPUT_OD;
            break;
        case Mode::input:
            config.Mode = GPIO_MODE_INPUT;
            break;
        default:
        case Mode::analog:
            break;
    }

    switch (info.speed) {
        default:
        case Speed::low:
            config.Speed = GPIO_SPEED_FREQ_LOW;
            break;
        case Speed::medium:
            config.Speed = GPIO_SPEED_FREQ_MEDIUM;
            break;
        case Speed::high:
            config.Speed = GPIO_SPEED_FREQ_HIGH;
            break;
    }

    switch (info.pullup) {
        default:
        case Pullup::none:
            config.Pull = GPIO_NOPULL;
            break;
        case Pullup::up:
            config.Pull = GPIO_PULLUP;
            break;
        case Pullup::down:
            config.Pull = GPIO_PULLDOWN;
            break;
    }

    config.Pin = pin_value(info);

    init_clk(info.port);

    def_t* port_res = port_to_def(info.port);

    if (port_res == nullptr) csp::error_callback(__FILE__, __LINE__);

    HAL_GPIO_Init(port_res, &config);
}

void deinit(const GpioInfo& info)
{
    pin_t pin = pin_value(info);
    def_t* port = port_to_def(info.port);

    if (port == nullptr) csp::error_callback(__FILE__, __LINE__);

    deinit_clk(info.port);

    HAL_GPIO_DeInit(port, pin);
}

void on(const GpioInfo& info)
{
    pin_t pin = pin_value(info);
    def_t* port = port_to_def(info.port);

    if (port == nullptr) csp::error_callback(__FILE__, __LINE__);

    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

void off(const GpioInfo& info)
{
    pin_t pin = pin_value(info);
    def_t* port = port_to_def(info.port);

    if (port == nullptr) csp::error_callback(__FILE__, __LINE__);

    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

void toggle(const GpioInfo& info)
{
    pin_t pin = pin_value(info);
    def_t* port = port_to_def(info.port);

    if (port == nullptr) csp::error_callback(__FILE__, __LINE__);

    HAL_GPIO_TogglePin(port, pin);
}

bool state(const GpioInfo& info)
{
    pin_t pin = pin_value(info);
    def_t* port = port_to_def(info.port);

    if (port == nullptr) csp::error_callback(__FILE__, __LINE__);

    return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET;
}

}  // namespace csp::gpio
