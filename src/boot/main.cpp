
/*
#include <bsp.hpp>
#include <os.hpp>
#include <lib/buffers/liner_buffer.hpp>

#define loop while(true)

void do_logic()
{
    static bsp::os::time::time_t start = bsp::os::time::current();
    static lib::buffers::liner_buffer<uint8_t, 256> temp_data;

    if ((bsp::os::time::current() - start) >= 500) {
        bsp::gpio::status::toggle();
        start = bsp::os::time::current();
    }

    auto status = static_cast<uint8_t>(bsp::usb::status());

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

int main()
{
    bsp::init();
    bsp::gpio::status::init();
    bsp::usb::init();
    bsp::os::init();

    loop {
        do_logic();
    }

    return 0;
}
*/

#include <csp.hpp>
#include <hal.hpp>

const std::uint32_t app_address = 0x8004000;

void jump_to_app()
{
    __disable_irq(); // запрещаем прерывания

    SCB->VTOR = app_address;
    auto int_vector_table = (uint32_t *)app_address;

    typedef void (*ResetVectorFunc)();
    auto reset_vector = (ResetVectorFunc)int_vector_table[1];


    reset_vector();
}

using status_led = csp::gpio::Gpio<
    csp::gpio::Port::_C,
    csp::gpio::Pin::_13,
    csp::gpio::Mode::PushPull>;

int main()
{
    csp::init();
    csp::rcc::init();

    status_led::init();

    status_led::toggle();
    csp::tick::delay(1000);
    status_led::toggle();
    csp::tick::delay(1000);
    status_led::toggle();
    csp::tick::delay(1000);

    jump_to_app();

    while (true);
}
