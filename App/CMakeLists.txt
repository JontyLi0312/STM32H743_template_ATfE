cmake_minimum_required(VERSION 3.22)

project(App LANGUAGES C ASM)

set(App_Inc
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
	${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
	${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx/Include
	${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
)

set(App_Src
	
)

add_library(App INTERFACE)

target_include_directories(App INTERFACE ${App_Inc})

target_sources(App INTERFACE ${App_Src})