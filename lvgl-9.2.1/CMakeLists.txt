# Set minimum required CMake version.
cmake_minimum_required(VERSION 3.22)

# Define the project name for organizational purposes.
project(LVGL-9.2.1 C)

# Create the static library target named "lvgl".
# This is the target name used for linking.
add_library(LVGL-9.2.1 STATIC)

# Find all LVGL source files recursively.
file(GLOB_RECURSE LVGL_SOURCES 
    "src/*.c"
    "src/core/*.c"
    "src/draw/*.c"
    "src/extra/*.c"
    "src/font/*.c"
    "src/hal/*.c"
    "src/misc/*.c"
    "src/widgets/*.c"
    "src/extra/layouts/*.c"
    "src/extra/libs/*.c"
    "src/extra/others/*.c"
    "src/extra/themes/*.c"
    "src/extra/widgets/*.c"
    "src/extra/layouts/flex/*.c"
    "src/extra/layouts/grid/*.c"
    "src/extra/libs/bmp/*.c"
    "src/extra/libs/ffmpeg/*.c"
    "src/extra/libs/freetype/*.c"
    "src/extra/libs/fsdrv/*.c"
    "src/extra/libs/gif/*.c"
    "src/extra/libs/png/*.c"
    "src/extra/libs/qrcode/*.c"
    "src/extra/libs/rlottie/*.c"
    "src/extra/libs/sjgp/*.c"
    "src/extra/others/fragment/*.c"
    "src/extra/others/gridnav/*.c"
    "src/extra/others/ime/*.c"
    "src/extra/others/imgfont/*.c"
    "src/extra/others/monkey/*.c"
    "src/extra/others/msg/*.c"
    "src/extra/others/snapshot/*.c"
    "src/extra/themes/basic/*.c"
    "src/extra/themes/default/*.c"
    "src/extra/themes/mono/*.c"
    "src/extra/widgets/animimg/*.c"
    "src/extra/widgets/calendar/*.c"
    "src/extra/widgets/chart/*.c"
    "src/extra/widgets/colorwheel/*.c"
    "src/extra/widgets/imgbtn/*.c"
    "src/extra/widgets/keyboard/*.c"
    "src/extra/widgets/led/*.c"
    "src/extra/widgets/list/*.c"
    "src/extra/widgets/menu/*.c"
    "src/extra/widgets/meter/*.c"
    "src/extra/widgets/msgbox/*.c"
    "src/extra/widgets/span/*.c"
    "src/extra/widgets/spinbox/*.c"
    "src/extra/widgets/spinner/*.c"
    "src/extra/widgets/tabview/*.c"
    "src/extra/widgets/tileview/*.c"
    "src/extra/widgets/win/*.c"
    "demos/music/*.c"
)
    
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
    ${CMAKE_CURRENT_SOURCE_DIR}        # Expose current dir for lvgl.h and lv_conf.h.
    ${CMAKE_CURRENT_SOURCE_DIR}/demos/music
    ${CMAKE_CURRENT_SOURCE_DIR}/porting     # Private include for porting files.
)

# [新增] 链接 lvgl 模块所依赖的其他模块 (例如 core)。
# 这会自动处理头文件路径和符号链接。
target_link_libraries(LVGL-9.2.1 PUBLIC Core_Config Drivers User_Drivers)

# Define LV_CONF_INCLUDE_SIMPLE to allow including "lv_conf.h" directly.
target_compile_definitions(LVGL-9.2.1 PUBLIC LV_CONF_INCLUDE_SIMPLE)