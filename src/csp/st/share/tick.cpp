#include <tick.hpp>
#include <hal.hpp>

namespace csp::tick
{
uint32_t current()
{
    return HAL_GetTick();
}

void delay(uint32_t ms)
{
    HAL_Delay(ms);
}
}
