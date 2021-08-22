set(STM32_CUBE_F4_PATH ${CMAKE_SOURCE_DIR}/toolchain/st/STM32CubeF4-1.25.2)

find_package(CMSIS COMPONENTS STM32F401CC REQUIRED)
find_package(HAL COMPONENTS STM32F4 RCC GPIO CORTEX TIM UART USART PCD LL_USB I2C DMA REQUIRED)

add_library(csp_f401xc_core INTERFACE)

target_sources(csp_f401xc_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/csp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rcc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/gpio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../share/tick.cpp
)

target_include_directories(csp_f401xc_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/csp/share
)

target_link_libraries(csp_f401xc_core INTERFACE
    HAL::STM32::F4::RCC
    HAL::STM32::F4::RCCEx
    HAL::STM32::F4::CORTEX
    HAL::STM32::F4::GPIO
    HAL::STM32::F4::TIM
    HAL::STM32::F4::DMA
    CMSIS::STM32::F401xC
    STM32::NoSys
    STM32::Nano
)

target_compile_definitions(csp_f401xc_core INTERFACE
    -DHAL_MODULE_ENABLED
    -DHAL_CORTEX_MODULE_ENABLED
    -DHAL_DMA_MODULE_ENABLED
    -DHAL_FLASH_MODULE_ENABLED
    -DHAL_EXTI_MODULE_ENABLED
    -DHAL_GPIO_MODULE_ENABLED
    -DHAL_PWR_MODULE_ENABLED
    -DHAL_TIM_MODULE_ENABLED
    -DHAL_RCC_MODULE_ENABLED
)

add_library(CSP::F401xC::CORE ALIAS csp_f401xc_core)

add_subdirectory(usb/cdc)
add_subdirectory(tick_source)
add_subdirectory(timer)
add_subdirectory(uart)
add_subdirectory(i2c)
