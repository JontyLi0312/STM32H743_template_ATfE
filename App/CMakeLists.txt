cmake_minimum_required(VERSION 3.22)

project(app LANGUAGES C ASM)

set(app_inc
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

set(app_src
	${CMAKE_CURRENT_SOURCE_DIR}/Src/map.c
)

add_library(app STATIC)

target_include_directories(app PUBLIC ${app_inc})

target_sources(app PRIVATE ${app_src})

target_link_libraries(app PRIVATE core_config drivers user_drivers)