add_library(lib_utils INTERFACE)

target_include_directories(lib_utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(LIB::UTILS ALIAS lib_utils)
