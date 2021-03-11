add_library(lib_buffers INTERFACE)

target_include_directories(lib_buffers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(LIB::BUFFERS ALIAS lib_buffers)
