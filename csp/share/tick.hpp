#ifndef CSP_SHARE_TICK
#define CSP_SHARE_TICK

#include <cinttypes>

namespace csp::tick
{
uint32_t current();
void delay(uint32_t ms);
}  // namespace csp::tick

#endif /* CSP_SHARE_TICK */
