#include <csp.hpp>
#include <hal.hpp>
#include <usb_device.h>
#include <usbd_cdc_if.h>

namespace csp::usb
{
void init(Number, Speed)
{
    MX_USB_DEVICE_Init();
}

void send(uint8_t data[], std::size_t size)
{
    CDC_Transmit_FS(data, size);
}
}

extern "C" {
    extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

    void Error_Handler(void)
    {
        csp::error_callback(__FILE__, __LINE__);
    }

    void OTG_FS_IRQHandler(void)
    {
        HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
    }
}
