cmake_minimum_required(VERSION 3.22)

project(freertos LANGUAGES C)

set(freertos_inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/CMSIS_RTOS_V2
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/portable/GCC/ARM_CM4F
)

set(freertos_src
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/croutine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/event_groups.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/stream_buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/tasks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/timers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/CMSIS_RTOS_V2/cmsis_os2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/portable/MemMang/heap_4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/portable/GCC/ARM_CM4F/port.c
)

add_library(freertos STATIC)

target_include_directories(freertos PUBLIC ${freertos_inc})

target_sources(freertos PRIVATE ${freertos_src})

target_link_libraries(freertos PRIVATE core_config drivers)