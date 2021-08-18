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

const csp::uart::Number mhz_number = csp::uart::Number::_1;
const csp::uart::TransferMode mhz_transfer_mode = csp::uart::TransferMode::Interrupt;
const csp::uart::Settings mhz_settings = {
    .speed = csp::uart::Speed::_9600,
    .word_length = csp::uart::WordLength::_8,
    .stop_bit = csp::uart::StopBit::_1,
    .parity = csp::uart::Parity::None
};

const std::uint16_t bme280_address = 0x76;

static std::uint8_t receive_data[16] = { 0x00 };
static std::size_t receive_size = 0;

void csp::uart::transmit_callback(Number number)
{
    if (number != mhz_number) return;
}

void csp::uart::receive_callback(Number number, std::size_t size)
{
    if (number != mhz_number and size == 0) return;
    receive_size = size;

    csp::uart::abort_receive(mhz_number, mhz_transfer_mode);
    csp::uart::receive(mhz_number, mhz_transfer_mode, receive_data, sizeof(receive_data));
}


void co2_test()
{
    static std::uint8_t data[] = { 0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 };

    csp::uart::transmit(mhz_number, mhz_transfer_mode, data, sizeof(data));
    os::task::delay(1000);

    csp::usb::transmit(usb_number, (const std::uint8_t*)"CO2 complete\r\n", 14);
}

void loop()
{
    static const char* data_to_send = "Hello, World\r\n";
    csp::usb::transmit(usb_number, (const std::uint8_t*)data_to_send, (std::size_t)strlen(data_to_send));
    status_led::toggle();

    std::uint8_t i2c_data_to_send = 0xD0;
    std::uint8_t i2c_data_to_read = 0x00;

    csp::i2c::transmit(i2c_number, i2c_transfer_mode, bme280_address, &i2c_data_to_send, 1);
    csp::i2c::receive(i2c_number, i2c_transfer_mode, bme280_address, &i2c_data_to_read, 1);

    co2_test();

    os::task::delay(100);
};

int main()
{
    csp::init();
    csp::rcc::init();
    csp::usb::device::cdc::init(usb_number);
    csp::i2c::init(i2c_number, i2c_mode);

    csp::uart::init(mhz_number, mhz_settings);
    csp::uart::receive(mhz_number, mhz_transfer_mode, receive_data, sizeof(receive_data));
    csp::uart::enable_interrupt(mhz_number);

    status_led::init();

    os::init();

    while(true) {
        loop();
    }
}
