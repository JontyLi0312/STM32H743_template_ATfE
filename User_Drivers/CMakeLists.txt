cmake_minimum_required(VERSION 3.22)

project(user_drivers LANGUAGES C ASM)

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
)

add_library(user_drivers INTERFACE)

target_include_directories(user_drivers INTERFACE ${User_Drivers_Inc})

target_sources(user_drivers INTERFACE ${User_Drivers_Src})
