
add_library(os_no_os INTERFACE)

target_sources(os_no_os INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/os.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/no_os_allocator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mutex.cpp
    ${CMAKE_SOURCE_DIR}/src/os/share/allocate.cpp
)

target_include_directories(os_no_os INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/os/share/
)

target_link_libraries(os_no_os INTERFACE
    CSP::${CHIP}::CORE
)

add_library(OS::NoOs ALIAS os_no_os)
