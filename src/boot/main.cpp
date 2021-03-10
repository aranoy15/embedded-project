
#include <bsp.hpp>
#include <os.hpp>
#include <cstring>

int main(void)
{
    bsp::init();
    bsp::gpio::status::init();
    bsp::usb::init();

    auto start = bsp::os::current();
    const uint8_t data[] = "Ok receive\n";

    for (;;) {
        if ((bsp::os::current() - start) >= 500) {
            bsp::gpio::status::toggle();
            start = bsp::os::current();
        }

        if (bsp::usb::count() > 0) {
            while (bsp::usb::count() > 0) {
                uint8_t b = 0;
                bsp::usb::read(b);
            }

            bsp::usb::send(data, sizeof(data));
        }
    }

    return 0;
}
