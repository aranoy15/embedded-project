
set(APP_TARGET app)

add_executable(${APP_TARGET} main.cpp)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set_property(TARGET ${APP_TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif()

#message(STATUS "Boot libs: ${BOOT_LIBS}")

target_include_directories(${APP_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${APP_TARGET} PRIVATE
        CSP::${CHIP}::CORE
        CSP::${CHIP}::TickSource::SysTick
        CSP::${CHIP}::DEVICE::USB::CDC
        CSP::${CHIP}::TIMER
        CSP::${CHIP}::I2C
        CSP::${CHIP}::UART
        LIB::TIME
        LIB::STREAM
        OS::NoOs
        )

#target_link_libraries(boot PRIVATE
#        ${BOOT_LIBS}
#        )

stm32_add_linker_script(${APP_TARGET} PRIVATE
        ${CMAKE_SOURCE_DIR}/toolchain/linker/${PROVIDER}/${CHIP}/gcc/${APP_ADDRESS}.ld
        )

add_custom_target(${APP_TARGET}.binhex ALL#TARGET ${APP_TARGET} ALL
        COMMAND ${STM32_TARGET_TRIPLET}-objcopy -Obinary ${CMAKE_BINARY_DIR}/src/${APP_TARGET}/${APP_TARGET}.elf ${CMAKE_BINARY_DIR}/${APP_TARGET}.bin
        COMMAND ${STM32_TARGET_TRIPLET}-objcopy -Oihex ${CMAKE_BINARY_DIR}/src/${APP_TARGET}/${APP_TARGET}.elf ${CMAKE_BINARY_DIR}/${APP_TARGET}.hex
        DEPENDS ${APP_TARGET}
        COMMENT "Generate bin and hex files"
        )
