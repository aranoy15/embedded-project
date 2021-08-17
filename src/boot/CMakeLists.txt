
set(BOOT_TARGET boot)

add_executable(${BOOT_TARGET} main.cpp)

set_property(TARGET ${BOOT_TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION True)

#message(STATUS "Boot libs: ${BOOT_LIBS}")

#target_include_directories(${BOOT_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/src)

#target_link_libraries(boot PRIVATE
#    ${BOOT_LIBS}
#)

target_link_libraries(${BOOT_TARGET} PRIVATE
        CSP::${CHIP}::CORE
        CSP::${CHIP}::DEVICE::USB::CDC
        CSP::${CHIP}::TickSource::SysTick
        OS::NoOs
        )

stm32_add_linker_script(${BOOT_TARGET} PRIVATE
    ${CMAKE_SOURCE_DIR}/toolchain/linker/${PROVIDER}/${CHIP}/gcc/${BOOT_ADDRESS}.ld
)

add_custom_target(${BOOT_TARGET}.binhex ALL
    COMMAND ${STM32_TARGET_TRIPLET}-objcopy -Obinary ${CMAKE_BINARY_DIR}/src/${BOOT_TARGET}/${BOOT_TARGET}.elf ${CMAKE_BINARY_DIR}/${BOOT_TARGET}.bin
    COMMAND ${STM32_TARGET_TRIPLET}-objcopy -Oihex ${CMAKE_BINARY_DIR}/src/${BOOT_TARGET}/${BOOT_TARGET}.elf ${CMAKE_BINARY_DIR}/${BOOT_TARGET}.hex
    DEPENDS ${BOOT_TARGET}
    COMMENT "Generate bin and hex files"
)

#add_custom_target(${TARGET}.size ALL
#    ${STM32_TARGET_TRIPLET}-size ${CMAKE_BINARY_DIR}/src/boot/boot.elf
#    DEPENDS boot
#)
