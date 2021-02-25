cmake_minimum_required(VERSION 3.15)

#SET(CMAKE_SYSTEM_NAME Generic)
#SET(CMAKE_SYSTEM_PROCESSOR ARM)

#set(STM32_TOOLCHAIN_PATH D:/toolchains/arm_gdb/bin)

#set(CMAKE_C_STANDARD 17)
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
find_package(HAL COMPONENTS STM32${FAMILY} RCC GPIO CORTEX TIM REQUIRED)
#find_package(FreeRTOS COMPONENTS ARM_CM3 REQUIRED)

add_executable(${TARGET} main.cpp)

set_property(TARGET ${TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION True)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#add_subdirectory(F103C8)

add_subdirectory(csp)

target_link_libraries(${TARGET}
    #HAL::STM32::F1::RCC
    #HAL::STM32::F1::RCCEx
    #HAL::STM32::F1::GPIO
    #HAL::STM32::F1::CORTEX
    #HAL::STM32::F1::TIM
    #HAL::STM32::F1::TIMEx
    #CMSIS::STM32::${CHIP}
    #STM32::NoSys
    csp
    #FreeRTOS::ARM_CM3
    #FreeRTOS::Heap::1
    #FreeRTOS::Timers
    #FreeRTOS::CMSIS_V2
)

#add_definitions(-DUSE_HAL_DRIVER)
#add_definitions(-D${STM32_CHIP})

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
