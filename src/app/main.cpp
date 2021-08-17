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

const csp::i2c::Number i2c_number = csp::i2c::Number::_1;
const csp::i2c::Mode i2c_mode = csp::i2c::Mode::Standard;
const csp::i2c::TransferMode i2c_transfer_mode = csp::i2c::TransferMode::Simple;

const std::uint16_t bme280_address = 0x76;

void loop()
{
    static const char* data_to_send = "Hello, World\r\n";
    csp::usb::transmit(usb_number, (const std::uint8_t*)data_to_send, (std::size_t)strlen(data_to_send));
    status_led::toggle();

    std::uint8_t i2c_data_to_send = 0xD0;
    std::uint8_t i2c_data_to_read = 0x00;

    csp::i2c::transmit(i2c_number, i2c_transfer_mode, bme280_address, &i2c_data_to_send, 1);
    csp::i2c::receive(i2c_number, i2c_transfer_mode, bme280_address, &i2c_data_to_read, 1);

    os::task::delay(100);
};

int main()
{
    csp::init();
    csp::rcc::init();
    csp::usb::device::cdc::init(usb_number);
    csp::i2c::init(i2c_number, i2c_mode);

    status_led::init();

    os::init();

    while(true) {
        loop();
    }
}
