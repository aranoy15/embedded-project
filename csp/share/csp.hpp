#ifndef CSP_SHARE_CSP
#define CSP_SHARE_CSP

#include <gpio.hpp>
#include <rcc.hpp>
#include <tick.hpp>
#include <flash.hpp>
#include <uart.hpp>
#include <usb.hpp>

namespace csp
{
void init();
void error_callback(const char* file, int line);
}

#endif /* CSP_SHARE_CSP */
