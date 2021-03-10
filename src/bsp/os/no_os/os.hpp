#ifndef BSP_OS_NO_OS_OS
#define BSP_OS_NO_OS_OS

#include <csp.hpp>

namespace bsp::os
{
using time_t = std::uint32_t;

void sleep(time_t ms);
time_t current();
}

#endif /* BSP_OS_NO_OS_OS */
