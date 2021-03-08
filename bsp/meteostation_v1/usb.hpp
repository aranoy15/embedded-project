#ifndef BSP_METEOSTATION_USB
#define BSP_METEOSTATION_USB

#include <csp/share/csp.hpp>
#include <cinttypes>

namespace bsp::usb
{
using number_t = csp::usb::Number;
constexpr number_t main_number = number_t::_1;

void init();
void deinit();
std::size_t count();
bool send(const std::uint8_t data[], std::size_t size);
bool read(std::uint8_t data[], std::size_t size);
bool read(std::uint8_t& data);
}

#endif /* BSP_METEOSTATION_USB */
