# Set minimum required CMake version.
cmake_minimum_required(VERSION 3.22)

# Define the project name for organizational purposes.
project(LVGL-9.2.1 C)

# Create the static library target named "lvgl".
# This is the target name used for linking.
add_library(LVGL-9.2.1 STATIC)

# Find all LVGL source files recursively.
file(GLOB_RECURSE LVGL_SOURCES "src/*.c")

# Add custom porting files.
list(APPEND LVGL_SOURCES
    "porting/lv_port_disp.c"
    "porting/lv_port_indev.c"
)

# Add all sources to the "lvgl" target.
target_sources(LVGL-9.2.1 PRIVATE ${LVGL_SOURCES})

# Set include directories for the "lvgl" target.
target_include_directories(LVGL-9.2.1
    PUBLIC
    .           # Expose current dir for lvgl.h and lv_conf.h.
    PRIVATE
    porting     # Private include for porting files.
)

# [新增] 链接 lvgl 模块所依赖的其他模块 (例如 core)。
# 这会自动处理头文件路径和符号链接。
target_link_libraries(LVGL-9.2.1 PUBLIC Core_Config Drivers User_Drivers)

# Define LV_CONF_INCLUDE_SIMPLE to allow including "lv_conf.h" directly.
target_compile_definitions(LVGL-9.2.1 PUBLIC LV_CONF_INCLUDE_SIMPLE)