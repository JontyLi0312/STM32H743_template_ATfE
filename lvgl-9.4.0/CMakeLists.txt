cmake_minimum_required(VERSION 3.22)

project(LVGL-9.4.0 C)

add_library(LVGL-9.4.0 STATIC)

file(GLOB_RECURSE LVGL_SOURCES 
    "src/*.c"
    "src/core/*.c"
    "src/display/*.c"
    "src/draw/*.c"
    "src/drivers/*.c"
    "src/font/*.c"
    "src/indev/*.c"
    "src/layouts/*.c"
    "src/libs/*.c"
    "src/misc/*.c"
    "src/osal/*.c"
    "src/others/*.c"
    "src/stdlib/*.c"
    "src/themes/*.c"
    "src/tick/*.c"
    "src/widgets/*.c"
)
    
list(APPEND LVGL_SOURCES
    "porting/lv_port_disp.c"
    "porting/lv_port_indev.c"
)

target_include_directories(LVGL-9.4.0
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/porting
)

target_sources(LVGL-9.4.0 PRIVATE ${LVGL_SOURCES})

target_link_libraries(LVGL-9.4.0 PUBLIC core_config drivers user_drivers)