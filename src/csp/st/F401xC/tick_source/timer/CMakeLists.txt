add_library(csp_f401xc_tick_source_timer INTERFACE)

target_sources(csp_f401xc_tick_source_timer INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/tick_source.cpp
        )

add_library(CSP::F401xC::TickSource::Timer ALIAS csp_f401xc_tick_source_timer)
