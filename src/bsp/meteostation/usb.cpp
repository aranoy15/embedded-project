#include <bsp/meteostation/usb.hpp>
#include <cstring>
#include <circular_buffer.hpp>

namespace
{
constexpr std::size_t queue_size = 256;
using data_t = lib::buffers::circular_buffer<uint8_t, queue_size>;
data_t rx_data;
}

namespace bsp::usb
{
void init()
{
    csp::usb::device::cdc::init(bsp::usb::main_number);
    csp::usb::start_receive(bsp::usb::main_number);
}

void deinit()
{
    csp::usb::stop_receive(bsp::usb::main_number);
}

std::size_t count()
{
    return rx_data.size();
}

bool send(const std::uint8_t data[], std::size_t size)
{
    return csp::usb::transmit(main_number, const_cast<std::uint8_t*>(data), size);
}

bool read(std::uint8_t data[], std::size_t size)
{
    if (size > rx_data.size() or rx_data.empty() or size > queue_size) return false;

    for (std::size_t i = 0; i < size; ++i) {
        data[i] = rx_data.get();
    }

    return true;
}

bool read(std::uint8_t& data)
{
    return read(&data, 1);
}
}

namespace csp::usb
{
void receive_callback(Number number, uint8_t data[], std::size_t size)
{
    if (number == csp::usb::Number::_1) {
        for (std::size_t i = 0; i < size; ++i) {
            if (not rx_data.full()) {
                rx_data.put(data[i]);
            }
        }
    }
}
}
