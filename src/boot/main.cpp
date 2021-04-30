
#include <bsp.hpp>
#include <os.hpp>
#include <cstring>
#include <lib/buffers/liner_buffer.hpp>
#include <lib/time/time.hpp>


int main(void)
{
    bsp::init();
    bsp::gpio::status::init();
    bsp::usb::init();
    bsp::os::init();

    bsp::os::time::time_t start = bsp::os::time::current();

    lib::buffers::liner_buffer<uint8_t, 256> temp_data;

    for (;;) {
        if ((bsp::os::time::current() - start) >= 500) {
            bsp::gpio::status::toggle();
            start = bsp::os::time::current();
        }

        uint8_t status = static_cast<uint8_t>(bsp::usb::status());

        if (bsp::usb::count() > 0) {
            while (bsp::usb::count() > 0) {
                uint8_t b = 0;

                if (bsp::usb::read(b)) {
                    temp_data.put(b);
                }
            }

            bsp::usb::send(temp_data.data(), temp_data.size());
            bsp::usb::send(&status, 1);
            temp_data.reset();
        }
    }

    return 0;
}
