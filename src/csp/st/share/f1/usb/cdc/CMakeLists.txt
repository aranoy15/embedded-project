find_package(DeviceUSB COMPONENTS CDC REQUIRED)

add_library(csp_f103xb_usb_device_cdc INTERFACE)

target_sources(csp_f103xb_usb_device_cdc INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/usb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/conf/usbd_conf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/conf/usbd_desc.c
)

target_include_directories(csp_f103xb_usb_device_cdc INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/conf
)

target_link_libraries(csp_f103xb_usb_device_cdc INTERFACE
    HAL::STM32::F1::PCD
    HAL::STM32::F1::PCDEx
    HAL::STM32::F1::LL_USB
    CSP::F103xB::CORE
    USB::DEVICE::CDC
)

target_compile_definitions(csp_f103xb_usb_device_cdc INTERFACE
    -DHAL_PCD_MODULE_ENABLED
)

add_library(CSP::F103xB::DEVICE::USB::CDC ALIAS csp_f103xb_usb_device_cdc)
