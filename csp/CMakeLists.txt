add_library(csp INTERFACE)

target_include_directories(csp INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(${PROVIDER}/${CHIP})
