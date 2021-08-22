//
// Created by Антон Федосеев on 07.08.2021.
//

#ifndef SRC_CSP_SHARE_I2C_TEMPLATE_HPP
#define SRC_CSP_SHARE_I2C_TEMPLATE_HPP

#include <cinttypes>

namespace csp::i2c
{
enum class Number {
    None = 0,
    _1,
    _2,
    _3,
    _4
};

enum class Mode {
    Standard,           // 100 kHz
    Fast                // 400 kHz
};

enum class TransferMode {
    Simple,
    Interrupt,
    Dma
};

bool init(Number number, Mode mode);
bool is_device_ready(Number number, std::uint16_t address);
bool transmit(Number number, TransferMode transfer_mode, std::uint16_t address, const std::uint8_t data[],
              std::size_t size);
bool enable_interrupt(Number number);
bool disable_interrupt(Number number);
bool enable_dma(Number number);
bool disable_dma(Number number);
bool receive(Number number, TransferMode transfer_mode, std::uint16_t address, std::uint8_t data[],
             std::size_t size);
bool abort_receive(Number number, TransferMode transfer_mode, std::uint16_t address);
bool abort_transmit(Number number, TransferMode transfer_mode, std::uint16_t address);
void transmit_callback(Number number);
void receive_callback(Number number);
}

#endif  // SRC_CSP_SHARE_I2C_TEMPLATE_HPP
