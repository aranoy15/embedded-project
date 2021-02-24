target_link_libraries(csp INTERFACE
    HAL::STM32::F1::RCC
    HAL::STM32::F1::GPIO
    HAL::STM32::F1::CORTEX
    HAL::STM32::F1::TIM
    HAL::STM32::F1::TIMEx
    CMSIS::STM32::${CHIP}
    STM32::NoSys
)

target_include_directories(csp INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(csp INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/csp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rcc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tick.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupts.cpp
)
