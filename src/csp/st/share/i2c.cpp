//
// Created by afedoseev on 23.06.2021.
//

#include <csp.hpp>
#include <hal.hpp>

using handler_t = I2C_HandleTypeDef;

extern handler_t* get_i2c_handler(csp::i2c::Number number);

bool csp::i2c::transmit(Number number, TransferMode transfer_mode, std::uint16_t address, const std::uint8_t *data,
                        std::size_t size)
{
    auto handler = get_i2c_handler(number);

    if (handler == nullptr) return false;

    address = (std::uint16_t)(address << 1);

    volatile HAL_StatusTypeDef result = HAL_ERROR;

    if (transfer_mode == TransferMode::Simple) {
        result = HAL_I2C_Master_Transmit(handler, address, const_cast<std::uint8_t*>(data), size, size * 10);
        csp::i2c::transmit_callback(number);
    } else if (transfer_mode == TransferMode::Interrupt) {
        result = HAL_I2C_Master_Transmit_IT(handler, address, const_cast<std::uint8_t*>(data), size);
    } /*else if (transfer_mode == TransferMode::Dma) {
        result = HAL_I2C_Master_Transmit_DMA(handler, address, const_cast<std::uint8_t*>(data), size);
    }*/

    return result == HAL_OK;
}

bool csp::i2c::receive(Number number, TransferMode transfer_mode, std::uint16_t address, std::uint8_t *data,
                       std::size_t size)
{
    auto handler = get_i2c_handler(number);

    if (handler == nullptr) return false;

    address = (std::uint16_t)(address << 1);

    HAL_StatusTypeDef result = HAL_ERROR;

    if (transfer_mode == TransferMode::Simple) {
        result = HAL_I2C_Master_Receive(handler, address, data, size, size * 10);
        csp::i2c::receive_callback(number);
    } else if (transfer_mode == TransferMode::Interrupt) {
        result = HAL_I2C_Master_Receive_IT(handler, address, data, size);
    } /* else if (transfer_mode == TransferMode::Dma) {
        result = HAL_I2C_Master_Receive_DMA(handler, address, data, size);
    }*/

    return result == HAL_OK;
}

bool csp::i2c::abort_transmit(Number number, TransferMode transfer_mode, std::uint16_t address)
{
    auto handler = get_i2c_handler(number);

    if (handler == nullptr) return false;

    address = (std::uint16_t)(address << 1);

    HAL_StatusTypeDef result = HAL_ERROR;

    if (transfer_mode == TransferMode::Simple) {
        result = HAL_OK;
    } else if (transfer_mode == TransferMode::Interrupt or transfer_mode == TransferMode::Dma) {
        result = HAL_I2C_Master_Abort_IT(handler, address);
    }

    return result == HAL_OK;
}

bool csp::i2c::abort_receive(Number number, TransferMode transfer_mode, std::uint16_t address)
{
    return csp::i2c::abort_transmit(number, transfer_mode, address);
}

__weak void csp::i2c::transmit_callback(Number number)
{
    (void)number;
}

__weak void csp::i2c::receive_callback(Number number)
{
    (void)number;
}
