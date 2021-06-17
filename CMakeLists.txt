cmake_minimum_required(VERSION 3.19)

if (IS_TESTS_BUILD)
    add_subdirectory(tests)
else()
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR ARM)

    set(CMAKE_CXX_STANDARD 20)

    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stm32-cmake/cmake/stm32_gcc.cmake)

    project(embedded-project CXX C ASM)

    if (BOARD STREQUAL "")
        message(FATAL_ERROR "Can't configure project. Not set BOARD variable.")
    endif()

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
    include(BOARD/${BOARD})

    add_subdirectory(src)
endif()
