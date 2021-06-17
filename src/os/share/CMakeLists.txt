add_library(bsp_os_interface INTERFACE)

target_include_directories(bsp_os_interface INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(BSP::Os::Include ALIAS bsp_os_interface)
