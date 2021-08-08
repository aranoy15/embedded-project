add_library(csp_f401xc_timer INTERFACE)

target_link_libraries(csp_f401xc_timer INTERFACE
        HAL::STM32::F4::TIM
        HAL::STM32::F4::TIMEx
        )

target_sources(csp_f401xc_timer INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp
        ${CMAKE_SOURCE_DIR}/src/csp/st/share/timer.cpp
        )

add_library(CSP::F401xC::TIMER ALIAS csp_f401xc_timer)
