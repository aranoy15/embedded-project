//
// Created by afedoseev on 17.08.2021.
//

#include <csp.hpp>
#include <hal.hpp>

using handler_t = I2C_HandleTypeDef;

using namespace csp::gpio;

namespace
{
handler_t hi2c1;

using i2c1_sda = Gpio<Port::_B, Pin::_7, Mode::AlternateOpenDrain, Speed::VeryHigh, Pullup::Up, AlternateFunction::I2c1>;
using i2c1_scl = Gpio<Port::_B, Pin::_6, Mode::AlternateOpenDrain, Speed::VeryHigh, Pullup::Up, AlternateFunction::I2c1>;
}

handler_t* get_i2c_handler(csp::i2c::Number number)
{
    using namespace csp::i2c;

    if (number == csp::i2c::Number::_1)
        return &hi2c1;

    return nullptr;
}

csp::i2c::Number get_i2c_number(handler_t* handler)
{
    using namespace csp::i2c;

    if (handler->Instance == I2C1)
        return csp::i2c::Number::_1;

    return csp::i2c::Number::None;
}

I2C_TypeDef* get_i2c_instance(csp::i2c::Number number)
{
    using namespace csp::i2c;

    if (number == csp::i2c::Number::_1)
        return I2C1;

    return nullptr;
}

bool csp::i2c::init(Number number, Mode mode)
{
    auto handler = get_i2c_handler(number);
    auto instance = get_i2c_instance(number);

    if (handler == nullptr or instance == nullptr)
        return false;

    handler->Instance = instance;

    if (mode == Mode::Standard)
        handler->Init.ClockSpeed = 100000;
    else if (mode == Mode::Fast)
        handler->Init.ClockSpeed = 400000;

    handler->Init.DutyCycle = I2C_DUTYCYCLE_2;
    handler->Init.OwnAddress1 = 0;
    handler->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    handler->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    handler->Init.OwnAddress2 = 0;
    handler->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    handler->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(handler) != HAL_OK) {
        csp::error_callback(__FILE__, __LINE__);
        return false;
    }

    return true;
}

namespace
{
    void hal_i2c_msp_init(handler_t* handler)
    {
        using namespace csp::i2c;

        auto number = get_i2c_number(handler);

        if (number == Number::None) return;

        if (number == csp::i2c::Number::_1) {
            __HAL_RCC_I2C1_CLK_ENABLE();

            i2c1_scl::init();
            i2c1_sda::init();
        }
    }

    void hal_i2c_msp_deinit(handler_t* handler)
    {
        using namespace csp::i2c;

        auto number = get_i2c_number(handler);

        if (number == Number::None) return;

        if (number == csp::i2c::Number::_1) {
            __HAL_RCC_I2C1_CLK_DISABLE();

            i2c1_scl::deinit();
            i2c1_sda::deinit();
        }
    }
}

extern "C" {
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
    hal_i2c_msp_init(hi2c);
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
    hal_i2c_msp_deinit(hi2c);
}
}