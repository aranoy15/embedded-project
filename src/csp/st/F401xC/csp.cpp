#include <csp.hpp>

#include <stm32f4xx_hal.h>

extern uint32_t* g_pfnVectors;

namespace csp
{
void init()
{
    __enable_irq();
    SCB->VTOR = uint32_t(&g_pfnVectors);

    HAL_Init();
}

}

__weak void csp::error_callback(const char* file, int line) 
{
    (void)file;
    (void)line;
}

extern "C" {
void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}


}
