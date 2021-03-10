#ifndef CSP_SHARE_RCC
#define CSP_SHARE_RCC

namespace csp::rcc
{
void init();
void increment_tick();
void systick_handler();
}

#endif /* CSP_SHARE_RCC */
