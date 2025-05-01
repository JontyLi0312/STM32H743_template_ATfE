cmake_minimum_required(VERSION 3.22)

project(core LANGUAGES C ASM)

set(Defines_Syms
				USE_HAL_DRIVER
				STM32H743xx
				USE_PWR_LDO_SUPPLY
				$<$<CONFIG:Debug>:DEBUG>
)

set(Core_Inc
				${CMAKE_CURRENT_SOURCE_DIR}/Inc
				${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
				${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
				${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
				${CMAKE_SOURCE_DIR}/User_Drivers/Inc
				${CMAKE_SOURCE_DIR}/App/Inc
				${CMAKE_SOURCE_DIR}/lvgl-8.3.10/porting
				${CMAKE_SOURCE_DIR}/lvgl-8.3.10/demos/music
				${CMAKE_SOURCE_DIR}/lvgl-8.3.10/demos/stress
)

set(Core_Src
				${CMAKE_CURRENT_SOURCE_DIR}/Src/dma2d.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/fmc.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/gpio.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/ltdc.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/main.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32h7xx_hal_msp.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32h7xx_it.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/system_stm32h7xx.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/usart.c
				${CMAKE_SOURCE_DIR}/startup_stm32h743xx.s
)

add_library(core INTERFACE)

target_compile_definitions(core INTERFACE ${Defines_Syms})

target_include_directories(core INTERFACE ${Core_Inc})

target_sources(core INTERFACE ${Core_Src})
