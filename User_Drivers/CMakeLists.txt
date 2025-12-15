cmake_minimum_required(VERSION 3.22)

project(user_drivers LANGUAGES C ASM)

set(user_drivers_inc
				${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

set(user_drivers_src
				${CMAKE_CURRENT_SOURCE_DIR}/Src/lcd_fonts.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/lcd_image.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/lcd_show.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/lcd_test.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/sdram.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/touch_800x480.c
				${CMAKE_CURRENT_SOURCE_DIR}/Src/touch_iic.c
                ${CMAKE_CURRENT_SOURCE_DIR}/Src/qspi_w25q64.c
)

add_library(user_drivers STATIC)

target_include_directories(user_drivers PUBLIC ${user_drivers_inc})

target_sources(user_drivers PRIVATE ${user_drivers_src})

target_link_libraries(user_drivers PRIVATE core_config drivers)
