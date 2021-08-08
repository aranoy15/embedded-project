#ifndef BSP_METEOSTATION_USB
#define BSP_METEOSTATION_USB

#include <csp.hpp>

namespace bsp::usb
{
using number_t = csp::usb::Number;
using status_t = csp::usb::Status;

constexpr number_t main_number = number_t::_1;

void init();
void deinit();
std::size_t count();
bool send(const std::uint8_t data[], std::size_t size);
bool read(std::uint8_t data[], std::size_t size);
bool read(std::uint8_t& data);
status_t status();
}

#endif /* BSP_METEOSTATION_USB */
