#include <usb.hpp>
#include <cstring>

namespace
{
std::uint8_t rx_data[255];
std::size_t rx_size = 0;
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
    return rx_size;
}

bool send(const std::uint8_t data[], std::size_t size)
{
    return csp::usb::transmit(main_number, const_cast<std::uint8_t*>(data), size);
}

bool read(std::uint8_t data[], std::size_t size)
{
    if (size > sizeof(rx_data) or rx_size == 0 or size > rx_size) return false;

    for (std::size_t i = 0; i < size; ++i) {
        data[i] = rx_data[i];
    }

    std::memmove(&rx_data[0], &rx_data[size], rx_size - size);
    rx_size -= size;

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
        if (rx_size < sizeof(rx_data)) {
            memcpy(&rx_data[rx_size], data, size);

            if ((rx_size + size) > sizeof(rx_data))
                rx_size = sizeof(rx_data);
            else
                rx_size += size;
        }
    }
}
}
