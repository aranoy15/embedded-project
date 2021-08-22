//
// Created by afedoseev on 22.06.2021.
//

#include <csp.hpp>
#include <hal.hpp>

using handler_t = UART_HandleTypeDef;
using instance_t = USART_TypeDef;

using namespace csp::gpio;

namespace
{
handler_t huart1;
handler_t huart2;

using uart1_tx = Gpio<Port::_A, Pin::_9, Mode::AlternatePushPull, Speed::VeryHigh, Pullup::None, AlternateFunction::Uart1>;
using uart1_rx = Gpio<Port::_A, Pin::_10, Mode::AlternatePushPull, Speed::VeryHigh, Pullup::None, AlternateFunction::Uart1>;

using uart2_tx = Gpio<Port::_A, Pin::_2, Mode::AlternatePushPull, Speed::VeryHigh, Pullup::None, AlternateFunction::Uart2>;
using uart2_rx = Gpio<Port::_A, Pin::_3, Mode::AlternatePushPull, Speed::VeryHigh, Pullup::None, AlternateFunction::Uart2>;
}


void uart_init_irq(csp::uart::Number number)
{
    using namespace csp::uart;

    switch (number) {
        case Number::_1:
            HAL_NVIC_SetPriority(USART1_IRQn, 6, 0);
            HAL_NVIC_EnableIRQ(USART1_IRQn);
            break;
        case Number::_2:
            HAL_NVIC_SetPriority(USART2_IRQn, 6, 0);
            HAL_NVIC_EnableIRQ(USART2_IRQn);
            break;
        default:
            break;
    }
}

void uart_deinit_irq(csp::uart::Number number)
{
    using namespace csp::uart;

    switch (number) {
        case Number::_1:
            HAL_NVIC_DisableIRQ(USART1_IRQn);
            break;
        case Number::_2:
            HAL_NVIC_DisableIRQ(USART2_IRQn);
            break;
        default:
            break;
    }
}

instance_t* get_instance(csp::uart::Number number)
{
    using namespace csp::uart;

    switch (number) {
        case Number::_1:
            return USART1;
        case Number::_2:
            return USART2;
        default:
            return nullptr;
    }
}

handler_t* get_uart_handler(csp::uart::Number number)
{
    using namespace csp::uart;

    switch (number) {
        case Number::_1:
            return &huart1;
        case Number::_2:
            return &huart2;
        default:
            return nullptr;
    }
}

csp::uart::Number get_uart_number(handler_t* handler)
{
    using namespace csp::uart;

    if (handler->Instance == USART1)
        return Number::_1;
    else if (handler->Instance == USART2)
        return Number::_2;

    return Number::None;
}

bool csp::uart::init(Number number, Settings settings)
{
    handler_t* handler = get_uart_handler(number);
    instance_t* instance = get_instance(number);

    if (handler == nullptr or instance == nullptr) return false;

    handler->Instance = instance;
    handler->Init.BaudRate = static_cast<uint32_t>(settings.speed);

    if (settings.word_length == WordLength::_7)
        return false;
    else if (settings.word_length == WordLength::_8)
        handler->Init.WordLength = UART_WORDLENGTH_8B;
    else if (settings.word_length == WordLength::_9)
        handler->Init.WordLength = UART_WORDLENGTH_9B;

    if (settings.stop_bit == StopBit::_0_5)
        return false; // unsupported
    else if (settings.stop_bit == StopBit::_1)
        handler->Init.StopBits = UART_STOPBITS_1;
    else if (settings.stop_bit == StopBit::_1_5)
        return false;
    else if (settings.stop_bit == StopBit::_2)
        handler->Init.StopBits = UART_STOPBITS_2;

    if (settings.parity == Parity::None)
        handler->Init.Parity = UART_PARITY_NONE;
    else if (settings.parity == Parity::Even)
        handler->Init.Parity = UART_PARITY_EVEN;
    else if (settings.parity == Parity::Odd)
        handler->Init.Parity = UART_PARITY_ODD;

    handler->Init.Mode = UART_MODE_TX_RX;

    if (settings.flow_control == FlowControl::None)
        handler->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    else if (settings.flow_control == FlowControl::Rts)
        handler->Init.HwFlowCtl = UART_HWCONTROL_RTS;
    else if (settings.flow_control == FlowControl::Cts)
        handler->Init.HwFlowCtl = UART_HWCONTROL_CTS;
    else if (settings.flow_control == FlowControl::RtsAndCts)
        handler->Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;

    handler->Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(handler) != HAL_OK) {
        csp::error_callback(__FILE__, __LINE__);
        return false;
    }

    return true;
}

extern void uart_irq_handler(handler_t* handler);

namespace {
    void hal_uart_msp_init(handler_t *handler) {
        using namespace csp::uart;

        auto number = get_uart_number(handler);

        if (number == Number::None) return;

        if (number == Number::_1) {
            __HAL_RCC_USART1_CLK_ENABLE();

            uart1_rx::init();
            uart1_tx::init();
        } else if (number == Number::_2) {

            __HAL_RCC_USART2_CLK_ENABLE();

            uart2_tx::init();
            uart2_rx::init();
        }
    }

    void hal_uart_msp_deinit(handler_t* handler)
    {
        using namespace csp::uart;

        auto number = get_uart_number(handler);

        if (number == Number::None) return;

        if (number == Number::_1) {
            __HAL_RCC_USART1_CLK_DISABLE();

            uart1_rx::deinit();
            uart1_tx::deinit();
        } else if (number == Number::_2) {
            __HAL_RCC_USART2_CLK_DISABLE();

            uart2_tx::deinit();
            uart2_rx::deinit();
        }
    }
}

extern "C" {
void HAL_UART_MspInit(UART_HandleTypeDef* handler)
{
    hal_uart_msp_init(handler);
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* handler)
{
    hal_uart_msp_deinit(handler);
}

void USART1_IRQHandler(void)
{
    uart_irq_handler(&huart1);
}

void USART2_IRQHandler(void)
{
    uart_irq_handler(&huart2);
}
}
