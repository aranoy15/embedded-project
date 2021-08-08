//
// Created by afedoseev on 24.06.2021.
//

#include <csp.hpp>
#include <hal.hpp>

namespace
{
[[maybe_unused]] TIM_HandleTypeDef htim1;
[[maybe_unused]] TIM_HandleTypeDef htim2;
[[maybe_unused]] TIM_HandleTypeDef htim3;
[[maybe_unused]] TIM_HandleTypeDef htim4;
}

using handler_t = TIM_HandleTypeDef;
using instance_t = TIM_TypeDef;

handler_t* get_timer_handler(csp::timer::Number number)
{
    using namespace csp::timer;

    switch (number) {
        case Number::_1:
            return &htim1;
        case Number::_2:
            return &htim2;
        case Number::_3:
            return &htim3;
        case Number::_4:
            return &htim4;
        default:
            return nullptr;
    }
}

csp::timer::Number get_timer_number(handler_t* htim)
{
    using namespace csp::timer;

    if (htim->Instance == TIM1)
        return Number::_1;
    else if (htim->Instance == TIM2)
        return Number::_2;
    else if (htim->Instance == TIM3)
        return Number::_3;
    else if (htim->Instance == TIM4)
        return Number::_4;

    return Number::None;
}

instance_t* get_timer_instance(csp::timer::Number number)
{
    using namespace csp::timer;

    switch (number) {
        case Number::_1:
            return TIM1;
        case Number::_2:
            return TIM2;
        case Number::_3:
            return TIM3;
        case Number::_4:
            return TIM4;
        default:
            return nullptr;
    }
}

csp::timer::frequency_t get_timer_frequency(csp::timer::Number number)
{
    using namespace csp::timer;

    RCC_ClkInitTypeDef clk_config;
    std::uint32_t latency;
    HAL_RCC_GetClockConfig(&clk_config, &latency);

    [[maybe_unused]] std::uint32_t pclk1_freq = HAL_RCC_GetPCLK1Freq();
    [[maybe_unused]] std::uint32_t pclk2_freq = HAL_RCC_GetPCLK2Freq();

    [[maybe_unused]] std::uint32_t apb1_timer = pclk1_freq * 2;
    [[maybe_unused]] std::uint32_t apb2_timer = pclk2_freq * 2;

    switch (number) {
        case Number::_1:
            return apb2_timer;
        default:
            break;
    }

    return 0xFFFFFFFF;
}

