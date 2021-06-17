set(STM32_CUBE_F1_PATH ${CMAKE_SOURCE_DIR}/toolchain/st/STM32CubeF1-1.8.3)

find_package(CMSIS COMPONENTS STM32F103C8 REQUIRED)
find_package(HAL COMPONENTS STM32F1 RCC GPIO CORTEX TIM FLASH UART USART PCD LL_USB REQUIRED)


add_library(csp_f103xb_core INTERFACE)

target_sources(csp_f103xb_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/csp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rcc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/flash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/gpio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/tick.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/flash.cpp
)

target_include_directories(csp_f103xb_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/csp/share
)

target_link_libraries(csp_f103xb_core INTERFACE
    HAL::STM32::F1::RCC
    HAL::STM32::F1::RCCEx
    HAL::STM32::F1::CORTEX
    HAL::STM32::F1::GPIO
    HAL::STM32::F1::FLASH
    HAL::STM32::F1::FLASHEx
    CMSIS::STM32::F103xB
    STM32::Nano
)

target_compile_definitions(csp_f103xb_core INTERFACE
    -DHAL_MODULE_ENABLED
    -DHAL_CORTEX_MODULE_ENABLED
    -DHAL_DMA_MODULE_ENABLED
    -DHAL_FLASH_MODULE_ENABLED
    -DHAL_EXTI_MODULE_ENABLED
    -DHAL_GPIO_MODULE_ENABLED
    -DHAL_PWR_MODULE_ENABLED
    -DHAL_RCC_MODULE_ENABLED
)

add_library(CSP::F103xB::CORE ALIAS csp_f103xb_core)

add_library(csp_uart INTERFACE)

target_sources(csp_uart INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/uart.cpp
)

target_include_directories(csp_uart INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/csp/share
)

target_link_libraries(csp_uart INTERFACE
    HAL::STM32::F1::UART
    HAL::STM32::F1::USART
)

target_compile_definitions(csp_uart INTERFACE
    -DHAL_UART_MODULE_ENABLED
    -DHAL_USART_MODULE_ENABLED
)

add_library(CSP::UART ALIAS csp_uart)

add_subdirectory(usb/cdc)
