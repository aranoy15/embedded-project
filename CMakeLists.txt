cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)

set(TARGET_TRIPLET arm-none-eabi)
set(STM32_CUBE_F1_PATH ${CMAKE_CURRENT_SOURCE_DIR}/toolchain/st/STM32CubeF1-1.8.3)
#set(FLASH_OFFSET 0x2000)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)

set(TARGET main)

project(${TARGET} CXX C ASM)

set(FAMILY F1)
set(CHIP F103RB)
set(PROVIDER st)

find_package(CMSIS COMPONENTS STM32${FAMILY} REQUIRED)
find_package(HAL COMPONENTS STM32${FAMILY} RCC GPIO CORTEX TIM UART USART REQUIRED)

add_executable(${TARGET} main.cpp)

set_property(TARGET ${TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION True)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(csp)

target_link_libraries(${TARGET} PRIVATE
    CSP::CORE
    CSP::UART
    #FreeRTOS::ARM_CM3
    #FreeRTOS::Heap::1
    #FreeRTOS::Timers
    #FreeRTOS::CMSIS_V2
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/tools.cmake)

add_custom_target(${TARGET}.generate_bin ALL
    arm-none-eabi-objcopy -Obinary ${CMAKE_BINARY_DIR}/${TARGET}.elf ${CMAKE_BINARY_DIR}/${TARGET}.bin
    DEPENDS ${TARGET}
)

add_custom_target(${TARGET}.size ALL
    arm-none-eabi-size ${CMAKE_BINARY_DIR}/${TARGET}.elf
    DEPENDS ${TARGET}
)

flash(${TARGET} 0x8000000)
