cmake_minimum_required(VERSION 3.22)

project(Drivers LANGUAGES C ASM)

set(Drivers_Defines_Syms
    STM32H743xx
    USE_HAL_DRIVER
)

set(Drivers_Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Include
)

set(Drivers_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_fmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_nor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sram.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_nand.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sdram.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dsi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_qspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_delayblock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
)

add_library(Drivers STATIC)

target_compile_definitions(Drivers PUBLIC ${Drivers_Defines_Syms})

target_include_directories(Drivers PUBLIC ${Drivers_Inc})

target_sources(Drivers PRIVATE ${Drivers_Src})

target_link_libraries(Drivers PRIVATE Core_Config)