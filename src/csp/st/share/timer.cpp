//
// Created by afedoseev on 24.06.2021.
//


#include <csp.hpp>
#include <hal.hpp>

using handler_t = TIM_HandleTypeDef;
using instance_t = TIM_TypeDef;

extern csp::timer::frequency_t get_timer_frequency(csp::timer::Number number);
extern handler_t* get_timer_handler(csp::timer::Number number);
extern instance_t* get_timer_instance(csp::timer::Number number);

bool csp::timer::init(Number number, Mode mode, frequency_t frequency)
{
    handler_t* htim = get_timer_handler(number);
    instance_t* itim = get_timer_instance(number);

    const frequency_t mhz_max = 1000000;

    if (htim == nullptr or itim == nullptr) {
        csp::error_callback(__FILE__, __LINE__);
        return false;
    }

    [[maybe_unused]] frequency_t timer_freq = get_timer_frequency(number);

    // #TODO TIMERS: realize normal prescaler and period calculation method
    frequency_t prescaler = (mhz_max / frequency) - 1;
    frequency_t period = (mhz_max / frequency) - 1;

    htim->Instance = itim;

    htim->Init.ClockDivision = 0;
    if (mode == Mode::CountUp)
        htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    else if (mode == Mode::CountDown)
        htim->Init.CounterMode = TIM_COUNTERMODE_DOWN;

    htim->Init.Prescaler = prescaler;
    htim->Init.Period = period;

    if (HAL_TIM_Base_Init(htim) != HAL_OK) {
        csp::error_callback(__FILE__, __LINE__);
        return false;
    }

    return true;
}

__weak void csp::timer::timer_callback(Number number)
{
    (void)number;
}
