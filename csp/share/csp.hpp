#ifndef CSP_SHARE_CSP
#define CSP_SHARE_CSP

#include <csp/share/gpio.hpp>
#include <csp/share/rcc.hpp>
#include <csp/share/tick.hpp>
#include <csp/share/flash.hpp>
#include <csp/share/uart.hpp>
#include <csp/share/usb.hpp>

namespace csp
{
void init();
void error_callback(const char* file, int line);
}

#endif /* CSP_SHARE_CSP */
