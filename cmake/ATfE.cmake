set(CMAKE_SYSTEM_NAME 		Generic)
set(CMAKE_SYSTEM_PROCESSOR 	arm)

set(CMAKE_C_COMPILER_ID 		Clang)
set(CMAKE_CXX_COMPILER_ID 		Clang)

# ATfE path must be set in here.
set(TOOLCHAIN_PREFIX	"D:/ARM_Toolchain/ATfE-20.1.0/bin/")

set(CMAKE_C_COMPILER 	${TOOLCHAIN_PREFIX}/clang.exe)
set(CMAKE_ASM_COMPILER 	${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER 	${TOOLCHAIN_PREFIX}/clang++.exe)
set(CMAKE_LINKER 		${TOOLCHAIN_PREFIX}/clang.exe)
set(CMAKE_OBJCOPY 		${TOOLCHAIN_PREFIX}/llvm-objcopy.exe)
set(CMAKE_SIZE 			${TOOLCHAIN_PREFIX}/llvm-size.exe)

set(CMAKE_EXECUTABLE_SUFFIX_ASM 	".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C 		".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX 	".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE 	STATIC_LIBRARY)

# MCU specific flags
set(TARGET_FLAGS 	"--target=armv7m-none-eabi  -mfpu=fpv5-d16 -mfloat-abi=hard")

set(CMAKE_C_FLAGS 	"${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MP")
set(CMAKE_C_FLAGS	"${CMAKE_C_FLAGS} -Wall -fdata-sections -ffunction-sections")

set(CMAKE_C_FLAGS_DEBUG 	"-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE 	"-Os -g0")
set(CMAKE_CXX_FLAGS_DEBUG 	"-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -g0")

set(CMAKE_CXX_FLAGS			"${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_CXX_FLAGS "${TARGET_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS 	"${CMAKE_EXE_LINKER_FLAGS} -lcrt0-hosted")
set(CMAKE_EXE_LINKER_FLAGS 	"${CMAKE_EXE_LINKER_FLAGS} -T \"${CMAKE_SOURCE_DIR}/STM32H743XX_FLASH.ld\"")
set(CMAKE_EXE_LINKER_FLAGS 	"${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_EXE_LINKER_FLAGS 	"${CMAKE_EXE_LINKER_FLAGS} -z noexecstack")
set(CMAKE_EXE_LINKER_FLAGS 	"${CMAKE_EXE_LINKER_FLAGS} -Wl,--print-memory-usage ")

