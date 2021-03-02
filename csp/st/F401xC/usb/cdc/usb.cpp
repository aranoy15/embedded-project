#include <csp.hpp>
#include <hal.hpp>
#include <usbd_cdc_if.h>
#include <usbd_desc.h>
#include <usbd_conf.h>
#include <usbd_def.h>
#include <usbd_core.h>

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
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

void HAL_PCD_MspInit(PCD_HandleTypeDef* pcdHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (pcdHandle->Instance == USB_OTG_FS) {

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USB_OTG_FS GPIO Configuration
        PA11     ------> USB_OTG_FS_DM
        PA12     ------> USB_OTG_FS_DP
        */
        GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

        /* Peripheral interrupt init */
        HAL_NVIC_SetPriority(OTG_FS_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
    }
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef* pcdHandle)
{
    if (pcdHandle->Instance == USB_OTG_FS) {
        /* Peripheral clock disable */
        __HAL_RCC_USB_OTG_FS_CLK_DISABLE();

        /**USB_OTG_FS GPIO Configuration
        PA11     ------> USB_OTG_FS_DM
        PA12     ------> USB_OTG_FS_DP
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

        /* Peripheral interrupt Deinit*/
        HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
    }
}

void Error_Handler(void)
{
    csp::error_callback(__FILE__, __LINE__);
}

void OTG_FS_IRQHandler(void)
{
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}
}
