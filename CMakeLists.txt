cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME STM32H743IIT6)

# Include toolchain file
# include("cmake/ATfE.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Core project settings
project(${CMAKE_PROJECT_NAME} LANGUAGES C ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32H743XX_FLASH.ld)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    LINK_DEPENDS ${LINKER_SCRIPT}
)

# Add STM32CubeMX generated sources
#add_subdirectory(cmake/stm32cubemx)
add_subdirectory(core)
add_subdirectory(user_drivers)
add_subdirectory(drivers)
add_subdirectory(app)
add_subdirectory(LVGL)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
	# Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
	# Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
	# Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
	# Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    core_config
	core
    drivers
	user_drivers
    app
    lvgl
	# Add user defined libraries
)

# Add the map file to the list of files to be removed with 'clean' target
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)

# Define paths for the output artifacts.
set(HEX_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex)
# Define separate paths for internal and external flash.
set(INTERNAL_ELF_FILE ${CMAKE_BINARY_DIR}/internal_flash.elf)
set(EXTERNAL_BIN_FILE ${CMAKE_BINARY_DIR}/external_flash.bin)

# Add a post-build step to generate the.hex file from the.elf output.
add_custom_command(
    OUTPUT ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
    DEPENDS ${PROJECT_NAME}
)

# Generate the.elf file for the internal flash.
add_custom_command(
    OUTPUT ${INTERNAL_ELF_FILE}
    COMMAND ${CMAKE_OBJCOPY} --remove-section=.qspi_section
            $<TARGET_FILE:${PROJECT_NAME}> ${INTERNAL_ELF_FILE}
    DEPENDS ${PROJECT_NAME}
)

# Generate the.bin file for the external QSPI flash.
add_custom_command(
    OUTPUT ${EXTERNAL_BIN_FILE}
    COMMAND ${CMAKE_OBJCOPY} -O binary --only-section=.qspi_section
            $<TARGET_FILE:${PROJECT_NAME}> ${EXTERNAL_BIN_FILE}
    DEPENDS ${PROJECT_NAME}
)

# Create a custom target to execute all file generation commands.  
add_custom_target(${PROJECT_NAME}_post_build ALL
    DEPENDS
        ${HEX_FILE}
        ${INTERNAL_ELF_FILE}
        ${EXTERNAL_BIN_FILE}
)