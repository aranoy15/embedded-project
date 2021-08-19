add_library(lib_data INTERFACE)

#target_include_directories(lib_data INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(LIB::DATA ALIAS lib_data)
