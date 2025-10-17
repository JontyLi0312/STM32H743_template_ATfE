cmake_minimum_required(VERSION 3.22)

project(Core LANGUAGES C ASM)

set(Core_Defines_Syms
	STM32H743xx
	USE_PWR_LDO_SUPPLY
	$<$<CONFIG:Debug>:DEBUG>
)

set(Core_Inc
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

set(Core_Src
	${CMAKE_CURRENT_SOURCE_DIR}/Src/dma2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/dma.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/fmc.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/gpio.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/ltdc.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/quadspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/rtc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/sdmmc.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32h7xx_hal_msp.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32h7xx_it.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/system_stm32h7xx.c
	${CMAKE_CURRENT_SOURCE_DIR}/Src/usart.c
	${CMAKE_SOURCE_DIR}/startup_stm32h743xx.s
)

add_library(Core STATIC)
add_library(Core_Config INTERFACE)

target_include_directories(Core_Config INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_compile_definitions(Core PRIVATE ${Core_Defines_Syms})

target_sources(Core PUBLIC ${Core_Src})

target_link_libraries(Core PRIVATE Core_Config Drivers User_Drivers App)