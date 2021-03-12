
#include <bsp.hpp>
#include <os.hpp>
#include <cstring>
#include <wake.hpp>

using protocol_t = lib::wake::Protocol<uint8_t, uint8_t, uint8_t, 256>;

int main(void)
{
    bsp::init();
    bsp::gpio::status::init();
    bsp::usb::init();
    bsp::os::init();

    bsp::os::time::time_t start = bsp::os::time::current();
    const uint8_t data[] = "Ok receive\n";

    for (;;) {
        if ((bsp::os::time::current() - start) >= 500) {
            bsp::gpio::status::toggle();
            start = bsp::os::time::current();
        }

        if (bsp::usb::count() > 0) {
            while (bsp::usb::count() > 0) {
                uint8_t b = 0;
                bsp::usb::read(b);

                protocol_t::process(b);
            }

            bsp::usb::send(data, sizeof(data));
        }
    }

    return 0;
}
