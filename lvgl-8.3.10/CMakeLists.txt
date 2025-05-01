cmake_minimum_required(VERSION 3.22)

add_library(lvgl INTERFACE)

set(LVGL_DIR "${CMAKE_SOURCE_DIR}/lvgl-8.3.10")
file(GLOB_RECURSE LVGL_SUBDIRS ${LVGL_DIR} "${LVGL_DIR}/*")
set(LVGL_INCLUDE_DIRS)

foreach(DIR ${LVGL_SUBDIRS})
    #if(IS_DIRECTORY ${LVGL_DIR}/${DIR} AND NOT ${DIR} MATCHES "demos")
    if(IS_DIRECTORY ${LVGL_DIR}/${DIR})
        list(APPEND LVGL_INCLUDE_DIRS ${LVGL_DIR}/${DIR})
    endif()
endforeach()

list(APPEND LVGL_INCLUDE_DIRS ${LVGL_DIR})

file(GLOB LVGL_Src
    #"${CMAKE_CURRENT_LIST_DIR}/demos/music/*.c"
    #"${CMAKE_CURRENT_LIST_DIR}/demos/music/assets/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/demos/stress/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/core/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/draw/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/misc/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/extra/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/font/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/hal/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/widgets/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/extra/layouts/grid/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/extra/layouts/flex/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/extra/widgets/**/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/extra/themes/**/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/draw/sw/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/porting/*.c"
)

target_include_directories(lvgl INTERFACE 
    ${LVGL_INCLUDE_DIRS}
)

target_sources(lvgl INTERFACE 
    ${LVGL_Src}
)