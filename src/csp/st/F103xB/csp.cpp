#include <csp.hpp>
#include <hal.hpp>

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

    __disable_irq();
    while (true);
}

extern "C" {
void HAL_MspInit(void)
{
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
     */
    __HAL_AFIO_REMAP_SWJ_NOJTAG();
}


}
