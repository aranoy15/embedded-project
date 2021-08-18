//
// Created by afedoseev on 21.06.2021.
//

#include <csp.hpp>
#include <hal.hpp>

using namespace csp::uart;

using handler_t = UART_HandleTypeDef;
using status_t = HAL_StatusTypeDef;
extern handler_t* get_uart_handler(Number number);
extern Number get_uart_number(handler_t* handler);
extern void uart_init_irq(Number number);
extern void uart_deinit_irq(Number number);

bool csp::uart::deinit(Number number)
{
    handler_t* huart = get_uart_handler(number);

    if (huart == nullptr) {
        csp::error_callback(__FILE__, __LINE__);
        return false;
    }

    if (HAL_UART_DeInit(huart) != HAL_OK)
        return false;

    return true;
}

bool csp::uart::enable_interrupt(Number number)
{
    handler_t* huart = get_uart_handler(number);

    if (huart == nullptr) {
        csp::error_callback(__FILE__, __LINE__);
        return false;
    }

    uart_init_irq(number);
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    return true;
}

bool csp::uart::disable_interrupt(Number number)
{
    handler_t* huart = get_uart_handler(number);

    if (huart == nullptr) {
        csp::error_callback(__FILE__, __LINE__);
        return false;
    }

    uart_deinit_irq(number);
    __HAL_UART_DISABLE_IT(huart, UART_IT_IDLE);
    return true;
}

bool csp::uart::transmit(Number number, TransferMode mode, const uint8_t *data, std::size_t size)
{
    handler_t* huart = get_uart_handler(number);

    if (huart == nullptr) return false;

    status_t result = HAL_ERROR;

    if (mode == TransferMode::Simple) {
        result = HAL_UART_Transmit(huart, const_cast<uint8_t*>(data), size, size * 10);
        csp::uart::transmit_callback(number);
    } else if (mode == TransferMode::Interrupt) {
        result = HAL_UART_Transmit_IT(huart, const_cast<uint8_t*>(data), size);
    } /*else if (mode == TransferMode::Dma) {
        result = HAL_UART_Transmit_DMA(huart, const_cast<uint8_t*>(data), size);
    }*/

    return result == HAL_OK;
}

bool csp::uart::receive(Number number, TransferMode mode, uint8_t *data, std::size_t size)
{
    handler_t* huart = get_uart_handler(number);

    if (huart == nullptr) return false;

    status_t result = HAL_ERROR;

    if (mode == TransferMode::Simple) {
        result = HAL_UART_Receive(huart, data, size, size * 10);
        csp::uart::receive_callback(number, size);
    } else if (mode == TransferMode::Interrupt) {
        result = HAL_UART_Receive_IT(huart, data, size);
    } /* else if (mode == TransferMode::Dma) {
        result = HAL_UART_Receive_DMA(huart, data, size);
    }*/

    return result == HAL_OK;
}


bool csp::uart::abort_receive(Number number, TransferMode mode)
{
    handler_t* huart = get_uart_handler(number);

    if (huart == nullptr) return false;

    status_t result = HAL_ERROR;

    if (mode == TransferMode::Interrupt /*or mode == TransferMode::Dma*/)
        result = HAL_UART_AbortReceive_IT(huart);

    return result == HAL_OK;
}

bool csp::uart::abort_transmit(Number number, TransferMode mode)
{
    handler_t* huart = get_uart_handler(number);

    if (huart == nullptr) return false;

    status_t result = HAL_ERROR;

    if (mode == TransferMode::Interrupt /*or mode == TransferMode::Dma*/)
        result = HAL_UART_AbortTransmit_IT(huart);

    return result == HAL_OK;
}

__weak void csp::uart::transmit_callback(Number number)
{
    (void)number;
}

__weak void csp::uart::receive_callback(Number number, std::size_t size)
{
    (void)number;
    (void)size;
}

__weak void csp::uart::receive_error_callback(Number number)
{
    (void)number;
}

void uart_irq_handler(handler_t* huart)
{
    HAL_UART_IRQHandler(huart);

    if (RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE)) {
        Number number = get_uart_number(huart);

        if (number != Number::None) {
            std::uint32_t size = huart->RxXferSize - huart->RxXferCount;
            if (size > 0) csp::uart::receive_callback(number, size);
        }

        __HAL_UART_CLEAR_IDLEFLAG(huart);
    }
}

namespace
{
    void hal_uart_tx_complete_callback(handler_t* handler)
    {
        auto number = get_uart_number(handler);

        if (number == Number::None) {
            csp::error_callback(__FILE__, __LINE__);
            return;
        }

        csp::uart::transmit_callback(number);
    }

    void hal_uart_rx_error_callback(handler_t* handler)
    {
        auto number = get_uart_number(handler);

        if (number == Number::None) {
            csp::error_callback(__FILE__, __LINE__);
            return;
        }

        csp::uart::receive_error_callback(number);
    }
}

extern "C" {

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    hal_uart_tx_complete_callback(huart);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    hal_uart_rx_error_callback(huart);
}

}
