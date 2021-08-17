
add_library(csp_f103xb_uart INTERFACE)

target_link_libraries(csp_f103xb_uart INTERFACE
        HAL::STM32::F1::UART
        )

target_sources(csp_f103xb_uart INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/uart.cpp
        ${CMAKE_SOURCE_DIR}/src/csp/st/share/uart.cpp
        )

add_library(CSP::F103xB::UART ALIAS csp_f103xb_uart)
