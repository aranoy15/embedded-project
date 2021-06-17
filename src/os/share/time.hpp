#ifndef SRC_BSP_OS_SHARE_TIME
#define SRC_BSP_OS_SHARE_TIME

#include <cinttypes>

namespace bsp::os::time
{
using time_t = std::uint32_t;

void sleep(time_t ms);
time_t current();
}

#endif /* SRC_BSP_OS_SHARE_TIME */