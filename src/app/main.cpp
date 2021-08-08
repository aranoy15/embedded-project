//
// Created by Антон Федосеев on 27.06.2021.
//

#include <csp.hpp>
#include <cstring>

#include <os.hpp>

using status_led = csp::gpio::Gpio<
    csp::gpio::Port::_C,
    csp::gpio::Pin::_13,
    csp::gpio::Mode::PushPull>;

const csp::usb::Number usb_number = csp::usb::Number::_1;

void loop()
{
    static const char* data_to_send = "Hello, World\r\n";
    csp::usb::transmit(usb_number, (const std::uint8_t*)data_to_send, (std::size_t)strlen(data_to_send));
    status_led::toggle();
    os::task::delay(100);
};

int main()
{
    csp::init();
    csp::rcc::init();
    csp::usb::device::cdc::init(usb_number);

    status_led::init();

    os::init();

    while(true) {
        loop();
    }
}
