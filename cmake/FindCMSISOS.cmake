
if (NOT CMSIS_OS_FIND_COMPONENTS)
    set(CMSIS_OS_FIND_COMPONENTS
        1 2
    )
endif()

if (NOT ${STM32_CUBE_${FAMILY}_PATH})
    set(${STM32_CUBE_${FAMILY}_PATH})
endif()
