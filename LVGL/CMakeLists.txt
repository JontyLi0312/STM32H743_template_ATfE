cmake_minimum_required(VERSION 3.22)

project(lvgl LANGUAGES C)

add_library(lvgl STATIC)

file(GLOB_RECURSE LVGL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/draw/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/extra/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/font/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gpu/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hal/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/misc/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/porting/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/demos/music/*.c"
)

target_include_directories(lvgl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/porting
    ${CMAKE_CURRENT_SOURCE_DIR}/demos/music
)

target_sources(lvgl PRIVATE ${LVGL_SOURCES})

target_link_libraries(lvgl PRIVATE core_config drivers user_drivers)