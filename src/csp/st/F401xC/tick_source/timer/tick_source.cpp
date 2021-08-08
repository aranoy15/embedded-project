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

TIM_HandleTypeDef htim1;

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
    RCC_ClkInitTypeDef    clkconfig;
    uint32_t              uwTimclock = 0;
    uint32_t              uwPrescalerValue = 0;
    uint32_t              pFLatency;
    /*Configure the TIM1 IRQ priority */
    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, TickPriority ,0);

    /* Enable the TIM1 global Interrupt */
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    /* Enable TIM1 clock */
    __HAL_RCC_TIM1_CLK_ENABLE();

    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* Compute TIM1 clock */
    uwTimclock = HAL_RCC_GetPCLK2Freq();
    /* Compute the prescaler value to have TIM1 counter clock equal to 1MHz */
    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

    /* Initialize TIM1 */
    htim1.Instance = TIM1;

    /* Initialize TIMx peripheral as follow:
    + Period = [(TIM1CLK/1000) - 1]. to have a (1/1000) s time base.
    + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
    + ClockDivision = 0
    + Counter direction = Up
    */
    htim1.Init.Period = (1000000U / 1000U) - 1U;
    htim1.Init.Prescaler = uwPrescalerValue;
    htim1.Init.ClockDivision = 0;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    if(HAL_TIM_Base_Init(&htim1) == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        return HAL_TIM_Base_Start_IT(&htim1);
    }

    /* Return function status */
    return HAL_ERROR;
}

void HAL_SuspendTick(void) {
    /* Disable TIM1 update Interrupt */
    __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);
}

void HAL_ResumeTick(void) {
    /* Enable TIM1 Update interrupt */
    __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
}


void TIM1_UP_TIM10_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1) {
        HAL_IncTick();
    } else {
        hal_timer_elapsed_handler(htim);
    }
}
}
