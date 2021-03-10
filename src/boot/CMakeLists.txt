
add_executable(boot main.cpp)

set_property(TARGET boot PROPERTY INTERPROCEDURAL_OPTIMIZATION True)

message("Boot libs: ${BOOT_LIBS}")

target_include_directories(boot PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(boot PRIVATE ${BOOT_LIBS})

stm32_add_linker_script(boot PRIVATE 
    ${CMAKE_SOURCE_DIR}/toolchain/linker/${PROVIDER}/${CHIP}/gcc/boot/${BOOT_ADDRESS}.ld
)

add_custom_target(boot.generate_bin ALL
    ${STM32_TARGET_TRIPLET}-objcopy -Obinary ${CMAKE_BINARY_DIR}/src/boot/boot.elf ${CMAKE_BINARY_DIR}/boot.bin
    DEPENDS boot
)

add_custom_target(${TARGET}.size ALL
    ${STM32_TARGET_TRIPLET}-size ${CMAKE_BINARY_DIR}/src/boot/boot.elf
    DEPENDS boot
)
