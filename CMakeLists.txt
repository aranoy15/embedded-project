cmake_minimum_required(VERSION 3.19)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR ARM)

set(CMAKE_CXX_STANDARD 20)

project(embedded-project CXX C ASM)

if (BOARD STREQUAL "")
    message(FATAL_ERROR "Can't configure project. Not set BOARD variable.")
endif()

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stm32-cmake/cmake/stm32_gcc.cmake)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#set(TARGET main)

#project(${TARGET} CXX C ASM)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BOARD/${BOARD}.cmake)

add_subdirectory(csp)
add_subdirectory(bsp)

if (HAVE_BOOT)
    add_subdirectory(boot)
endif()
