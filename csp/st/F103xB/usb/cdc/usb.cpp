#include <csp.hpp>
#include <hal.hpp>

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"

USBD_HandleTypeDef hUsbDeviceFS;

namespace 
{
constexpr std::size_t rx_size = 1000;
constexpr std::size_t tx_size = 1000;

uint8_t user_rx_buffer[rx_size];
uint8_t user_tx_buffer[tx_size];

bool need_receive_callback = false;

USBD_CDC_LineCodingTypeDef linecoding = {
    115200, /* baud rate*/
    0x00, /* stop bits-1*/
    0x00, /* parity - none*/
    0x08 /* nb. of bits 8*/
};

int8_t cdc_init_fs()
{
    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, user_tx_buffer, 0);
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, user_rx_buffer);
    return (USBD_OK);
}

int8_t cdc_deinit_fs(void)
{
    return (USBD_OK);
}

int8_t cdc_control_fs(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
    switch (cmd) {
        case CDC_SEND_ENCAPSULATED_COMMAND:

            break;

        case CDC_GET_ENCAPSULATED_RESPONSE:

            break;

        case CDC_SET_COMM_FEATURE:

            break;

        case CDC_GET_COMM_FEATURE:

            break;

        case CDC_CLEAR_COMM_FEATURE:

            break;

            /*******************************************************************************/
            /* Line Coding Structure */
            /*-----------------------------------------------------------------------------*/
            /* Offset | Field       | Size | Value  | Description */
            /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits
             * per second*/
            /* 4      | bCharFormat |   1  | Number | Stop bits */
            /*                                        0 - 1 Stop bit */
            /*                                        1 - 1.5 Stop bits */
            /*                                        2 - 2 Stop bits */
            /* 5      | bParityType |  1   | Number | Parity */
            /*                                        0 - None */
            /*                                        1 - Odd */
            /*                                        2 - Even */
            /*                                        3 - Mark */
            /*                                        4 - Space */
            /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or
             * 16).          */
            /*******************************************************************************/
        case CDC_SET_LINE_CODING:
            linecoding.bitrate = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |
                                            (pbuf[2] << 16) | (pbuf[3] << 24));
            linecoding.format = pbuf[4];
            linecoding.paritytype = pbuf[5];
            linecoding.datatype = pbuf[6];

            /* Add your code here */
            break;

        case CDC_GET_LINE_CODING:
            pbuf[0] = (uint8_t)(linecoding.bitrate);
            pbuf[1] = (uint8_t)(linecoding.bitrate >> 8);
            pbuf[2] = (uint8_t)(linecoding.bitrate >> 16);
            pbuf[3] = (uint8_t)(linecoding.bitrate >> 24);
            pbuf[4] = linecoding.format;
            pbuf[5] = linecoding.paritytype;
            pbuf[6] = linecoding.datatype;

            /* Add your code here */
            break;

        case CDC_SET_CONTROL_LINE_STATE:

            break;

        case CDC_SEND_BREAK:

            break;

        default:
            break;
    }

    return (USBD_OK);
}

int8_t cdc_receive_fs(uint8_t* buffer, uint32_t* size)
{
    if (need_receive_callback) {
        csp::usb::receive_callback(csp::usb::Number::_1, buffer, *size);
    }

    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &buffer[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);
    return (USBD_OK);
}

int8_t cdc_transmit_fs(uint8_t* buffer, uint16_t size)
{
    uint8_t result = USBD_OK;

    USBD_CDC_HandleTypeDef* hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
    if (hcdc->TxState != 0) {
        return USBD_BUSY;
    }

    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, buffer, size);
    result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);

    return result;
}

USBD_CDC_ItfTypeDef usbd_interface_fops_fs = {
    cdc_init_fs,
    cdc_deinit_fs,
    cdc_control_fs,
    cdc_receive_fs 
};
}

namespace csp::usb
{
namespace device::cdc
{
    void init(Number)
    {
        if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK) {
            csp::error_callback(__FILE__, __LINE__);
        }

        if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK) {
            csp::error_callback(__FILE__, __LINE__);
        }

        if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &usbd_interface_fops_fs) != USBD_OK) {
            csp::error_callback(__FILE__, __LINE__);
        }

        if (USBD_Start(&hUsbDeviceFS) != USBD_OK) {
            csp::error_callback(__FILE__, __LINE__);
        }
    }
}  // namespace device::cdc

void transmit(Number, uint8_t data[], std::size_t size)
{
    cdc_transmit_fs(data, size);
}

void start_receive(Number)
{
    need_receive_callback = true;
}

void stop_receive(Number)
{
    need_receive_callback = false;
}

__weak void receive_callback(Number, uint8_t*, std::size_t)
{
    csp::error_callback(__FILE__, __LINE__);
}

}  // namespace csp::usb

extern "C" {

extern PCD_HandleTypeDef hpcd_USB_FS;

void HAL_PCD_MspInit(PCD_HandleTypeDef* pcdHandle)
{
    if (pcdHandle->Instance == USB) {
        /* Peripheral clock enable */
        __HAL_RCC_USB_CLK_ENABLE();

        /* Peripheral interrupt init */
        HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 9, 0);
        HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    }
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* pcdHandle)
{
    if (pcdHandle->Instance == USB) {
        /* Peripheral clock disable */
        __HAL_RCC_USB_CLK_DISABLE();

        /* Peripheral interrupt Deinit*/
        HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
    }
}

void Error_Handler(void)
{
    csp::error_callback(__FILE__, __LINE__);
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    HAL_PCD_IRQHandler(&hpcd_USB_FS);
}
}
