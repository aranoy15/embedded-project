#include <csp.hpp>
#include <hal.hpp>

namespace
{
using pin_t = uint16_t;

pin_t pin_value(const csp::gpio::GpioInfo& info)
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
std::uint8_t gpio_get_alternate_function(csp::gpio::AlternateFunction function);

void init(const GpioInfo& info) noexcept
{
    GPIO_InitTypeDef config;

    switch (info.mode) {
        case Mode::PushPull:
            config.Mode = GPIO_MODE_OUTPUT_PP;
            break;
        case Mode::OpenDrain:
            config.Mode = GPIO_MODE_OUTPUT_OD;
            break;
        case Mode::Input:
            config.Mode = GPIO_MODE_INPUT;
            break;
        case Mode::AlternateOpenDrain:
            config.Mode = GPIO_MODE_AF_OD;
        default:
        case Mode::Analog:
            break;
    }

    config.Speed = speed_parse(info.speed);

    switch (info.pullup) {
        default:
        case Pullup::None:
            config.Pull = GPIO_NOPULL;
            break;
        case Pullup::Up:
            config.Pull = GPIO_PULLUP;
            break;
        case Pullup::Down:
            config.Pull = GPIO_PULLDOWN;
            break;
    }

    config.Pin = pin_value(info);

    init_clk(info.port);

    def_t* port_res = port_to_def(info.port);

    if (port_res == nullptr) csp::error_callback(__FILE__, __LINE__);

    HAL_GPIO_Init(port_res, &config);
}

void deinit(const GpioInfo& info) noexcept
{
    pin_t pin = pin_value(info);
    def_t* port = port_to_def(info.port);

    if (port == nullptr) csp::error_callback(__FILE__, __LINE__);

    deinit_clk(info.port);

    HAL_GPIO_DeInit(port, pin);
}

void on(const GpioInfo& info) noexcept
{
    pin_t pin = pin_value(info);
    def_t* port = port_to_def(info.port);

    if (port == nullptr) csp::error_callback(__FILE__, __LINE__);

    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

void off(const GpioInfo& info) noexcept
{
    pin_t pin = pin_value(info);
    def_t* port = port_to_def(info.port);

    if (port == nullptr) csp::error_callback(__FILE__, __LINE__);

    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

void toggle(const GpioInfo& info) noexcept
{
    pin_t pin = pin_value(info);
    def_t* port = port_to_def(info.port);

    if (port == nullptr) csp::error_callback(__FILE__, __LINE__);

    HAL_GPIO_TogglePin(port, pin);
}

bool state(const GpioInfo& info) noexcept
{
    pin_t pin = pin_value(info);
    def_t* port = port_to_def(info.port);

    if (port == nullptr) csp::error_callback(__FILE__, __LINE__);

    return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET;
}

}
