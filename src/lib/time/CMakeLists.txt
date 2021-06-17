add_library(lib_time INTERFACE)

target_include_directories(lib_time INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(lib_time INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/time.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp
)

add_library(LIB::TIME ALIAS lib_time)
