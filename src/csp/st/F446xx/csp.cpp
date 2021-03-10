#include <csp.hpp>

#include <stm32f4xx_hal.h>

namespace csp
{
void init()
{
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
