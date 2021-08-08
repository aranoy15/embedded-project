#ifndef CSP_SHARE_TICK
#define CSP_SHARE_TICK

#include <cinttypes>

namespace csp::tick
{
using tick_t = std::uint32_t;
tick_t current();
void delay(tick_t ms);
}  // namespace csp::tick

#endif /* CSP_SHARE_TICK */
