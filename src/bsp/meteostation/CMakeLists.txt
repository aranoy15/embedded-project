add_library(bsp_core INTERFACE)

target_sources(bsp_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp.cpp
)

target_include_directories(bsp_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/bsp/share
)

target_link_libraries(bsp_core INTERFACE
    CSP::${CHIP}::CORE
    CSP::${CHIP}::TickSource::SysTick
)

add_library(BSP::CORE ALIAS bsp_core)

add_library(bsp_usb INTERFACE)

target_sources(bsp_usb INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/usb.cpp
)

target_include_directories(bsp_usb INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(bsp_usb INTERFACE
    CSP::${CHIP}::DEVICE::USB::CDC
    LIB::DATA
    BSP::CORE
)

add_library(BSP::USB ALIAS bsp_usb)


