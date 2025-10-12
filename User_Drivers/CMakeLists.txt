cmake_minimum_required(VERSION 3.22)

project(User_Drivers LANGUAGES C ASM)

set(User_Drivers_Inc
				${CMAKE_SOURCE_DIR}/Core/Inc
				${CMAKE_SOURCE_DIR}/App/Inc
				${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver/Inc
				${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

set(User_Drivers_Src
				${CMAKE_CURRENT_SOURCE_DIR}/Src/lcd_fonts.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/lcd_image.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/lcd_show.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/lcd_test.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/sdram.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/touch_800x480.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/touch_iic.c
                ${CMAKE_CURRENT_SOURCE_DIR}/Src/qspi_w25q64.c
)

add_library(User_Drivers INTERFACE)

target_include_directories(User_Drivers INTERFACE ${User_Drivers_Inc})

target_sources(User_Drivers INTERFACE ${User_Drivers_Src})
