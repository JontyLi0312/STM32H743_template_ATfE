cmake_minimum_required(VERSION 3.22)

project(core LANGUAGES C ASM)

set(core_defines_syms
	STM32H743xx
	USE_PWR_LDO_SUPPLY
	$<$<CONFIG:Debug>:DEBUG>
)

set(core_inc
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

set(core_src
	${CMAKE_CURRENT_SOURCE_DIR}/Src/dma2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/dma.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/fmc.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/gpio.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/ltdc.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/quadspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/rtc.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32h7xx_hal_msp.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32h7xx_it.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/system_stm32h7xx.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/usart.c
	${CMAKE_SOURCE_DIR}/startup_stm32h743xx.s
)

add_library(core STATIC)

add_library(core_config INTERFACE)

target_include_directories(core_config INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_compile_definitions(core PRIVATE ${core_defines_syms})

target_sources(core PUBLIC ${core_src})

target_link_libraries(core PRIVATE core_config drivers user_drivers app LVGL-9.4.0)