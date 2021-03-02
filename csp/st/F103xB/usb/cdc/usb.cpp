#include <csp.hpp>
#include <hal.hpp>

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

USBD_HandleTypeDef hUsbDeviceFS;


namespace csp::usb
{
void init(Number)
{
    if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK) {
        csp::error_callback(__FILE__, __LINE__);
    }

    if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK) {
        csp::error_callback(__FILE__, __LINE__);
    }

    if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS) != USBD_OK) {
        csp::error_callback(__FILE__, __LINE__);
    }

    if (USBD_Start(&hUsbDeviceFS) != USBD_OK) {
        csp::error_callback(__FILE__, __LINE__);
    }
}

void send(Number, uint8_t data[], std::size_t size)
{
    CDC_Transmit_FS(data, size);
}
}

extern "C" {

extern PCD_HandleTypeDef hpcd_USB_FS;

void Error_Handler(void)
{
    csp::error_callback(__FILE__, __LINE__);
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    HAL_PCD_IRQHandler(&hpcd_USB_FS);
}
}
