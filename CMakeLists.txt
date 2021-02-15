cmake_minimum_required(VERSION 3.15)

#SET(CMAKE_SYSTEM_NAME Generic)
#SET(CMAKE_SYSTEM_PROCESSOR ARM)

set(STM32_TOOLCHAIN_PATH D:/toolchains/arm_gdb/bin)
set(TARGET_TRIPLET arm-none-eabi)
#set(FLASH_OFFSET 0x2000)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)

set(TARGET main)

project(${TARGET} CXX C ASM)

stm32_fetch_cmsis(F1)
stm32_fetch_hal(F1)

find_package(CMSIS COMPONENTS STM32F1 REQUIRED)
find_package(HAL COMPONENTS STM32F1 RCC GPIO CORTEX REQUIRED)
#find_package(FreeRTOS COMPONENTS ARM_CM3 REQUIRED)


add_executable(${TARGET} main.cpp)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET}
    HAL::STM32::F1::RCC
    HAL::STM32::F1::GPIO
    HAL::STM32::F1::CORTEX
    CMSIS::STM32::F103RB
    STM32::NoSys
    #FreeRTOS::ARM_CM3
    #FreeRTOS::Heap::4
)

#add_definitions(-DUSE_HAL_DRIVER)
#add_definitions(-D${STM32_CHIP})

add_custom_target(${TARGET}.generate_bin ALL
    arm-none-eabi-objcopy -Obinary ${CMAKE_BINARY_DIR}/${TARGET}.elf ${CMAKE_BINARY_DIR}/${TARGET}.bin
    DEPENDS ${NAME}
)

add_custom_target(${TARGET}.size ALL
    arm-none-eabi-size ${CMAKE_BINARY_DIR}/${TARGET}.elf
    DEPENDS ${NAME}
)

