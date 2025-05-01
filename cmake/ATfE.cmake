set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)

# Some default ATfE settings
# ATfE must be part of path environment
# Windows
if(WIN32)
set(TOOLCHAIN_PREFIX                "D:/Arm_ToolChain/ATfE-20.1.0-Windows-x86_64/bin" CACHE PATH "Path to the toolchain prefix")
set(CMAKE_C_COMPILER                "${TOOLCHAIN_PREFIX}/clang.exe")
set(CMAKE_ASM_COMPILER              "${CMAKE_C_COMPILER}")
set(CMAKE_CXX_COMPILER              "${TOOLCHAIN_PREFIX}/clang++.exe")
set(CMAKE_LINKER                    "${TOOLCHAIN_PREFIX}/ld.lld.exe")
set(CMAKE_OBJCOPY                   "${TOOLCHAIN_PREFIX}/llvm-objcopy.exe")
set(CMAKE_SIZE                      "${TOOLCHAIN_PREFIX}/jllvm-size.exe")
# Linux
elseif(UNIX)
set(TOOLCHAIN_PREFIX                "/home/jonty/Dev_Tool/ATfE-20.1.0-Linux-x86_64/bin" CACHE PATH "Path to the toolchain prefix")
set(CMAKE_C_COMPILER                "${TOOLCHAIN_PREFIX}/clang")
set(CMAKE_ASM_COMPILER              "${CMAKE_C_COMPILER}")
set(CMAKE_CXX_COMPILER              "${TOOLCHAIN_PREFIX}/clang++")
set(CMAKE_LINKER                    "${TOOLCHAIN_PREFIX}/ld.lld")
set(CMAKE_OBJCOPY                   "${TOOLCHAIN_PREFIX}/llvm-objcopy")
set(CMAKE_SIZE                      "${TOOLCHAIN_PREFIX}/jllvm-size")
else()
	message(FATAL_ERROR "Unsuppotred OS")
endif()

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")


set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# MCU specific flags
set(TARGET_FLAGS "--target=armv7m-none-eabi -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -fdata-sections -ffunction-sections")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-Os -g0")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -g0")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_C_LINK_FLAGS "${TARGET_FLAGS}")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T \"${CMAKE_SOURCE_DIR}/STM32H743XX_FLASH.ld\"")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--specs=nosys.specs")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -Wl,--end-group")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--print-memory-usage")

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lstdc++ -lsupc++ -Wl,--end-group")
