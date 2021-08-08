#include <hal.hpp>
#include <tick_template.hpp>

namespace csp::tick
{
tick_t current()
{
    return HAL_GetTick();
}

void delay(tick_t ms)
{
    HAL_Delay(ms);
}
}
