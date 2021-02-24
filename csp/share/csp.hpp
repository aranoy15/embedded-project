#ifndef CSP_SHARE_CSP
#define CSP_SHARE_CSP

#include <share/gpio.hpp>
#include <share/uart.hpp>
#include <share/tick.hpp>

namespace csp
{
    void error_callback(const char* file, int line);
}

#endif /* CSP_SHARE_CSP */
