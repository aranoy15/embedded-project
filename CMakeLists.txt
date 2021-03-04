cmake_minimum_required(VERSION 3.19)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR ARM)

set(CMAKE_CXX_STANDARD 17)

set(STM32_CUBE_F1_PATH ${CMAKE_CURRENT_SOURCE_DIR}/toolchain/st/STM32CubeF1-1.8.3)
set(STM32_CUBE_F4_PATH ${CMAKE_CURRENT_SOURCE_DIR}/toolchain/st/STM32CubeF4-1.25.2)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stm32-cmake/cmake/stm32_gcc.cmake)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(TARGET main)

project(${TARGET} CXX C ASM)

#set(FAMILY F4)
#set(CHIP F446xx)
#set(FAMILY F4)
#set(CHIP F401xC)
set(FAMILY F1)
set(CHIP F103xB)
set(PROVIDER st)

find_package(CMSIS COMPONENTS STM32${FAMILY} REQUIRED)

add_executable(${TARGET} main.cpp)

set_property(TARGET ${TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION True)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

stm32_add_linker_script(${TARGET} PRIVATE 
    ${CMAKE_SOURCE_DIR}/toolchain/linker/${PROVIDER}/${CHIP}/gcc/0x8000000.ld
)

add_subdirectory(csp)

target_compile_options(${TARGET} PRIVATE
)

target_link_libraries(${TARGET} PRIVATE
    CSP::CORE
    CSP::DEVICE::USB::CDC
    #CSP::UART
    #FreeRTOS::ARM_CM3
    #FreeRTOS::Heap::1
    #FreeRTOS::Timers
    #FreeRTOS::CMSIS_V2
)


add_custom_target(${TARGET}.generate_bin ALL
    ${STM32_TARGET_TRIPLET}-objcopy -Obinary ${CMAKE_BINARY_DIR}/${TARGET}.elf ${CMAKE_BINARY_DIR}/${TARGET}.bin
    DEPENDS ${TARGET}
)

add_custom_target(${TARGET}.size ALL
    ${STM32_TARGET_TRIPLET}-size ${CMAKE_BINARY_DIR}/${TARGET}.elf
    DEPENDS ${TARGET}
)
