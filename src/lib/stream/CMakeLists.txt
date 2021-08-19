add_library(lib_stream INTERFACE)

target_sources(lib_stream INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/writer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/actions.cpp
        )

add_library(LIB::STREAM ALIAS lib_stream)