find_package(DeviceUSB COMPONENTS CDC REQUIRED)

add_library(csp_usb_device_cdc INTERFACE)

target_sources(csp_usb_device_cdc INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/usb.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/usb_device.c
    ${CMAKE_CURRENT_SOURCE_DIR}/usbd_cdc_if.c
    ${CMAKE_CURRENT_SOURCE_DIR}/usbd_conf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/usbd_desc.c
)

target_include_directories(csp_usb_device_cdc INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(csp_usb_device_cdc INTERFACE
    HAL::STM32::F1::PCD
    HAL::STM32::F1::PCDEx
    HAL::STM32::F1::LL_USB
    CSP::CORE
    USB::DEVICE::CDC
)

target_compile_definitions(csp_usb_device_cdc INTERFACE
    -DHAL_PCD_MODULE_ENABLED
)

add_library(CSP::DEVICE::USB::CDC ALIAS csp_usb_device_cdc)
