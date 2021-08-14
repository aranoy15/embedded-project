//
// Created by afedoseev on 24.06.2021.
//

#include <csp.hpp>
#include <hal.hpp>

using handler_t = TIM_HandleTypeDef;

extern csp::timer::Number get_timer_number(handler_t* htim);

namespace
{
    void hal_timer_elapsed_handler(TIM_HandleTypeDef* htim)
    {
        using namespace csp::timer;

        Number number = get_timer_number(htim);

        if (number == Number::None) {
            csp::error_callback(__FILE__, __LINE__);
        }

        csp::timer::timer_callback(number);
    }
}

extern "C" {

void SysTick_Handler(void)
{
    HAL_IncTick();
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    hal_timer_elapsed_handler(htim);
}

}
