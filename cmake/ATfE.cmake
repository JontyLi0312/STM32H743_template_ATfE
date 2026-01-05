set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_SYSTEM_PROCESSOR  arm)
set(CMAKE_C_COMPILER_ID     Clang)
set(CMAKE_CXX_COMPILER_ID   Clang)

# ATfE toolchain path
set(TOOLCHAIN_PREFIX    "D:/Toolchain/ATfE-21.1.1/bin/")
set(CMAKE_C_COMPILER    ${TOOLCHAIN_PREFIX}/clang.exe)
set(CMAKE_ASM_COMPILER  ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER  ${TOOLCHAIN_PREFIX}/clang++.exe)
set(CMAKE_LINKER        ${TOOLCHAIN_PREFIX}/clang.exe)
set(CMAKE_OBJCOPY       ${TOOLCHAIN_PREFIX}/llvm-objcopy.exe)
set(CMAKE_SIZE          ${TOOLCHAIN_PREFIX}/llvm-size.exe)

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C   ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ============================================================
# MCU specific flags - 包含完整的FPU配置
# ============================================================
set(TARGET_FLAGS "\
--target=arm-none-eabi \
-mcpu=cortex-m7 \
-mfpu=fpv5-d16 \
-mfloat-abi=hard \
-mthumb \
")

# ============================================================
# 通用编译标志
# ============================================================
set(COMMON_FLAGS "\
${TARGET_FLAGS} \
-fdata-sections \
-ffunction-sections \
-fno-common \
-fomit-frame-pointer \
")

# ============================================================
# C语言标志
# ============================================================
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Debug配置
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3 -DDEBUG")

# Release配置 - 添加激进优化
set(CMAKE_C_FLAGS_RELEASE "\
-O3 \
-g0 \
-DNDEBUG \
-ffast-math \
-funroll-loops \
-fvectorize \
-fslp-vectorize \
")

# ============================================================
# C++语言标志（修复覆盖问题）
# ============================================================
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

# Debug配置
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -DDEBUG")

# Release配置
set(CMAKE_CXX_FLAGS_RELEASE "\
-O3 \
-g0 \
-DNDEBUG \
-ffast-math \
-funroll-loops \
-fvectorize \
-fslp-vectorize \
")

# ============================================================
# 汇编标志
# ============================================================
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -x assembler-with-cpp")

# ============================================================
# 链接器标志（关键修复！）
# ============================================================
if(NOT ATFE_CMAKE_INCLUDED)
    # 1. 必须包含TARGET_FLAGS以传递FPU/ABI信息
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${TARGET_FLAGS}")
    
    # 2. C运行时库
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lcrt0-hosted")
    
    # 3. 链接脚本
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T\"${CMAKE_SOURCE_DIR}/STM32H743XX_FLASH.ld\"")
    
    # 4. 垃圾回收和优化
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--print-memory-usage")
    
    # 5. 安全标志
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -z noexecstack")
    
    # 6. 生成map文件
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map")
    
    # 7. 数学库（picolibc需要显式链接）
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm")
    
    set(ATFE_CMAKE_INCLUDED TRUE)
endif()