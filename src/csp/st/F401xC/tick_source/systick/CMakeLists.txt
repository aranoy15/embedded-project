add_library(csp_f401xc_tick_source_systick INTERFACE)

target_sources(csp_f401xc_tick_source_systick INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/tick_source.cpp
        )

add_library(CSP::F401xC::TickSource::SysTick ALIAS csp_f401xc_tick_source_systick)
