
find_package(HAL COMPONENTS STM32F4 RCC GPIO CORTEX TIM UART USART PCD LL_USB REQUIRED)


add_library(csp_core INTERFACE)

target_sources(csp_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/csp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rcc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/gpio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/tick.cpp
)

target_include_directories(csp_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/csp/share
)

target_link_libraries(csp_core INTERFACE
    HAL::STM32::F4::RCC
    HAL::STM32::F4::RCCEx
    HAL::STM32::F4::CORTEX
    HAL::STM32::F4::GPIO
    HAL::STM32::F4::LL_USB
    CMSIS::STM32::F401xC
    STM32::NoSys
)

target_compile_definitions(csp_core INTERFACE
    -DHAL_MODULE_ENABLED
    -DHAL_CORTEX_MODULE_ENABLED
    -DHAL_DMA_MODULE_ENABLED
    -DHAL_FLASH_MODULE_ENABLED
    -DHAL_EXTI_MODULE_ENABLED
    -DHAL_GPIO_MODULE_ENABLED
    -DHAL_PWR_MODULE_ENABLED
    -DHAL_RCC_MODULE_ENABLED
)

add_library(CSP::CORE ALIAS csp_core)

add_subdirectory(usb/cdc)
