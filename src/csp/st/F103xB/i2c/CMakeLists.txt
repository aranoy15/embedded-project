add_library(csp_f103xb_i2c INTERFACE)

target_link_libraries(csp_f103xb_i2c INTERFACE
        HAL::STM32::F1::I2C
        )

target_sources(csp_f103xb_i2c INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/i2c.cpp
        ${CMAKE_SOURCE_DIR}/src/csp/st/share/i2c.cpp
        )

add_library(CSP::F103xB::I2C ALIAS csp_f103xb_i2c)
