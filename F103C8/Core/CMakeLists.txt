target_include_directories(${TARGET} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32f1xx_hal_msp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32f1xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32f1xx_hal_timebase_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/main.c
)
