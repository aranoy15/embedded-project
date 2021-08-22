
add_library(csp_f401xc_uart INTERFACE)

target_link_libraries(csp_f401xc_uart INTERFACE
        HAL::STM32::F4::UART
        )

target_sources(csp_f401xc_uart INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/uart.cpp
        ${CMAKE_SOURCE_DIR}/src/csp/st/share/uart.cpp
        )

target_compile_definitions(csp_f401xc_uart INTERFACE
        -DHAL_UART_MODULE_ENABLED
        )

add_library(CSP::F401xC::UART ALIAS csp_f401xc_uart)
