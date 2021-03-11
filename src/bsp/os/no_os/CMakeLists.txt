
add_library(bsp_os_no_os INTERFACE)

target_sources(bsp_os_no_os INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/os.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/time.cpp
)

target_include_directories(bsp_os_no_os INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(bsp_os_no_os INTERFACE
    CSP::${CHIP}::CORE
)

add_library(BSP::NoOs ALIAS bsp_os_no_os)
