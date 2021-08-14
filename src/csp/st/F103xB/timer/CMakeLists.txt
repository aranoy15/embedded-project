add_library(csp_f103xb_timer INTERFACE)

target_link_libraries(csp_f103xb_timer INTERFACE
        HAL::STM32::F1::TIM
        HAL::STM32::F1::TIMEx
        )

target_sources(csp_f103xb_timer INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp
        ${CMAKE_SOURCE_DIR}/src/csp/st/share/timer.cpp
        )

target_compile_definitions(csp_f103xb_timer INTERFACE
        )

add_library(CSP::F103xB::TIMER ALIAS csp_f103xb_timer)
