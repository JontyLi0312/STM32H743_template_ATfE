cmake_minimum_required(VERSION 3.22)

project(App LANGUAGES C ASM)

set(App_Inc
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

set(App_Src
	${CMAKE_CURRENT_SOURCE_DIR}/Src/map.c
)

add_library(App STATIC)

target_include_directories(App PUBLIC ${App_Inc})

target_sources(App PRIVATE ${App_Src})

target_link_libraries(App PRIVATE Core_Config Drivers User_Drivers)