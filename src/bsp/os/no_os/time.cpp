#include <bsp/os/no_os/time.hpp>
#include <csp.hpp>

namespace bsp::os::time
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
