cmake_minimum_required(VERSION 3.22)

project(lvgl LANGUAGES C)

file(GLOB_RECURSE lvgl_src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/display/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/draw/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/font/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/indev/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/layouts/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/misc/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osal/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/others/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stdlib/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/themes/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tick/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/porting/lv_port_disp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/porting/lv_port_indev.c
    ${CMAKE_CURRENT_SOURCE_DIR}/demos/music/*.c
)

add_library(lvgl STATIC)

target_include_directories(lvgl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/porting
    ${CMAKE_CURRENT_SOURCE_DIR}/demos/music
)

target_sources(lvgl PRIVATE ${lvgl_src})

target_link_libraries(lvgl PRIVATE core_config drivers user_drivers app)