#include <bsp/os/no_os/os.hpp>

namespace bsp::os
{
void sleep(time_t ms)
{
    csp::tick::delay(ms);
}

time_t current()
{
    return static_cast<time_t>(csp::tick::current());
}
}

namespace csp::rcc
{
void systick_handler()
{
    csp::rcc::increment_tick();
}
}
