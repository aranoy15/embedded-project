add_library(lib_wake INTERFACE)

target_sources(lib_wake INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/wake.cpp
)

target_include_directories(lib_wake INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lib_wake INTERFACE LIB::BUFFERS)

add_library(LIB::WAKE ALIAS lib_wake)
