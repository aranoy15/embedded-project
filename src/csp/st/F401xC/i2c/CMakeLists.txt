add_library(csp_f401xc_i2c INTERFACE)

target_link_libraries(csp_f401xc_i2c INTERFACE
        HAL::STM32::F4::I2C
        )

target_sources(csp_f401xc_i2c INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/i2c.cpp
        ${CMAKE_SOURCE_DIR}/src/csp/st/share/i2c.cpp
        )

target_compile_definitions(csp_f401xc_i2c INTERFACE
        -DHAL_I2C_MODULE_ENABLED
        )

add_library(CSP::F401xC::I2C ALIAS csp_f401xc_i2c)
