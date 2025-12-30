# STM32H743IIT6 项目模板 (基于 ATfE)

[English](./docs/README_en.md) | **中文**

## 📋 概述（Overview）

本项目采用 Arm 基于 LLVM 及 picolibc 定制的 **Arm Toolchain for Embedded (ATfE)** 编译器。项目初始化代码由 STM32CubeMX 生成，通过引入 **CMake** 构建系统和 **Clangd** LSP，实现了跨平台的一致性开发体验与高效的代码索引能力。

### ✨ 特性（Features）

  * **现代化工具链**：采用 Clangd 与 ATfE。
  * **跨平台构建支持**：使用 CMake 与 Ninja，可以在 Windows, macOS, Linux 上获得一致的构建体验。
  * **VS Code 集成**：预置 `.vscode/tasks.json` 和 `.vscode/launch.json`，实现快捷的编译、烧录和调试。
  * **模块化工程结构**：结构清晰，易于集成业务代码、中间件或 RTOS。
  * **DMA 内存管理**：定义专用 RAM 内存段（`.ram_section`），解决了 Cache 一致性问题。

### 🔌 硬件平台（Hardware Platform）

  * **开发板 (Board)**：反客科技 STM32H743IIT6 最小系统板。
  * **显示屏 (Display)**：RGB 接口 TFT 液晶屏。
  * **引脚配置 (Pinout)**：详见项目根目录下的 `.ioc` 文件（需要使用 STM32CubeMX 查看）。

## 🚀 快速开始（Quick Start）

默认使用 **Visual Studio Code** 编辑器。

### 🛠️ 环境依赖（Prerequisites）

请确保以下工具已正确安装，并建议将其路径添加至系统 `PATH` 环境变量。

| 工具 (Tool)           | 说明 (Description)                                                 | 链接 (Link)                                                                                                 |
| :-------------------- | :----------------------------------------------------------------- | :---------------------------------------------------------------------------------------------------------- |
| **CMake**             | 构建系统生成器                                                     | [cmake.org](https://cmake.org/)                                                                             |
| **Ninja**             | 高速构建工具                                                       | [github.com/ninja-build/ninja](https://github.com/ninja-build/ninja)                                        |
| **OpenOCD**           | 开源调试与烧录工具                                                 | [github.com/openocd-org/openocd](https://github.com/openocd-org/openocd)                                    |
| **pyOCD**             | 用于调试、编程和探索 Arm Cortex 微控制器的基于 Python 的工具和 API | [github.com/pyocd/pyOCD](https://github.com/pyocd/pyOCD)                                                    |
| **ATfE**              | Arm 官方嵌入式工具链 (基于 LLVM)                                   | [arm.com/toolchain](https://github.com/arm/arm-toolchain/blob/arm-software/arm-software/embedded/README.md) |
| **Arm GNU Toolchain** | 提供 GDB 调试器                                                    | [arm.com/gnu-toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)                  |
| **LLVM**              | 提供 `clangd` LSP                                                  | [github.com/llvm/llvm-project](https://github.com/llvm/llvm-project)                                        |

有关 ATfE 的详细特性，参阅[Arm Toolchain for Embedded Professional 文档](https://developer.arm.com/documentation/107976/21-1-1/?lang=en)。

### 📦 VS Code 插件（VS Code Extensions）

1. **Cortex-Debug**：用于调试。
2. **Clangd**：用于代码补全、语法检查和定义跳转。
3. **CMake Tools**：用于 CMake 的配置和管理。

### ⚙️ 初始化配置（Init Config）

1. 克隆仓库到本地。
2. 打开 `cmake/ATfE.cmake` 文件。
3. 修改 `TOOLCHAIN_PREFIX` 变量，使其指向本地 **ATfE** 根目录。

## 🏗️ 构建与部署 (Build & Deploy)

### 任务流（Workflow）

项目集成了 VS Code 自动化任务（`Ctrl+Shift+B`）以简化开发流程。

基础任务（Basic Tasks）：执行单一操作的基本任务。
1. **🔧 Configure**：配置 CMake 环境。通常 `🔨 Build` 任务会自动调用它，无需手动执行。
2. **🔨 Build**：编译。
3. **🧹 Clean**：清理构建生成的文件。
4. **📥 Flash (OpenOCD)**：使用 OpenOCD（CMSIS-DAP）将固件烧录到目标设备，若需要使用其他 Link，请修改 task 配置。 
5. **📥 Flash (pyOCD)**：使用 pyOCD （任意受 pyOCD 支持的Link）将固件烧录到目标设备。

组合工作流 (Composite Workflows): 按顺序执行多个基础任务的便捷工作流。
1. **🧹-> 🔨**: 按顺序自动执行清理和编译两个步骤。
2. **🔨-> 📥 (OpenOCD/pyOCD)**: 按顺序执行编译，并使用 OpenOCD 或 pyOCD 进行烧录。
3. **🧹-> 🔨 -> 📥 (OpenOCD/pyOCD)**: 按顺序执行清理、编译，并使用 OpenOCD 或 pyOCD 进行烧录。

所有任务的详细定义均可在 `.vscode/tasks.json` 文件中查看和修改。

### 🔨 编译产物（Artifacts）

通过在顶层 CMakeLists.txt 中添加如下命令，构建系统会自动处理内存映射，生成表格所示的文件。
```cmake
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
```
| 文件（File）       | 说明（Description）                      |
| :----------------- | :--------------------------------------- |
| project.hex        | 标准 Intel HEX 文件                      |
| internal_flash.elf | 仅包含内部 Flash 数据的 ELF 文件         |
| external_flash.bin | 仅包含外部 QSPI Flash 数据的纯二进制文件 |

### 📥 外部 Flash 烧录（External Flash Programming）

针对外部 QSPI Flash 资源，建议使用 pyOCD 配合定制的下载算法（FLM）进行烧录：
```
pyocd flash -t stm32h743iitx build/Debug/internal_flash.elf build/Debug/external_flash.bin@0x90000000
```

**⚠️警告：** 使用本项目提供的 `.flm` 算法前请确保目标硬件的 QSPI 引脚配置与本项目的相同，否则需要自行准备 `.flm` 算法。

## 🔬 调试 (Debugging)

### 标准调试（Standard Debugging）
1. **GDB 依赖**: 本项目的调试功能依赖 **Arm GNU Toolchain** 中的 GDB。
2. **调试配置**: 所有的调试配置都预设在 `.vscode/launch.json` 文件中。
3. **启动调试**:
   * 在 VS Code 的侧边栏切换到“Run and Debug”视图。
   * 根据你使用的调试器，选择 `openocd_CMSIS-DAP`、`openocd_ST-Link` 或 `pyOCD` 配置启动调试。

> **注意**: 如果使用了外部 flash，此时 pyocd 调试不可用，请切换至 openocd 或者下述的 ozone。

### ozone 调试（Ozone Debug）

ozone 是 Segger 推出的图形化调试器，可以使用 JLink 调试 STM32 项目，如果希望使用 CMSIS-DAP 等其他 Link，ozone 也提供了 GDB 的选项，通过 OpenOCD 或 pyOCD 连接至 MCU 后，在 ozone 中可以通过 GDB 进行调试，该功能目前需要申请证书（URL: https://www.segger.com/purchase/licensing/license-request/）。

## 💡 CMake 预设（CMake Preset）

项目有两种预设：
| 配置 （Profile） | 优化等级 （Optimization） |
| :--------------- | :------------------------ |
| **Debug**        | -O0 -g3                   |
| **Release**      | -Os -g0                   |

## ⚠️ 内存一致性与缓存（Memory Coerency）

为确保 **DMA** 的可访问性，本项目在链接脚本（`.ld`）中新增了一个位于 AXI SRAM (`0x24000000`) 的内存段 `.ram_section`。


使用方法：
1. 声明数据：
    ```c
    __attribute__(section(".ram_section"))) __attribute__((aligned(32))) uint8_t my_dma_buffer[1024];
    ```
2. 维护一致性。若开启了 D-Cache，在 CPU 修改了该区域的数据后、启动 DMA 传输前，**必须**执行缓存清理：
    ```c
    SCB_CleanDCache_by_Addr((uint32_t*)my_dma_buffer, sizeof(my_dma_buffer));
    ```

## 📁 项目结构（Project Struct）

```
STM32H743IIT6_template_ATfE/
├─📁 .conf-------------------- # 项目配置文件
├─📁 .vscode------------------ # VS Code 配置文件
├─📁 App---------------------- # 业务逻辑代码
├─📁 cmake-------------------- # CMake 配置文件
├─📁 Core--------------------- # STM32CubeMX 生成的核心代码
├─📁 docs--------------------- # 文档
├─📁 Drivers------------------ # STM32CubeMX 生成的驱动代码
├─📁 lvgl-9.4.0--------------- # LVGL 图形库
├─📁 User_Drivers------------- # 用户驱动代码
├─📄 .clang-format------------ # clang 格式化配置
├─📄 .clangd------------------ # clangd 配置
├─📄 .gitignore--------------- # Git 忽略文件
├─📄 .mxproject--------------- # STM32CubeMX 配置文件
├─📄 CMakeLists.txt----------- # 顶层 CMake 配置文件
├─📄 CMakePresets.json-------- # CMake 预设文件
├─📄 pyocd_user.py------------ # pyOCD 配置文件
├─📄 README.md---------------- # README
├─📄 startup_stm32h743xx.s---- # stm32h743 启动文件
├─📄 STM32H743IIT6.ioc-------- # STM32CubeMX 工程
└─📄 STM32H743XX_FLASH.ld----- # 链接脚本
```

## 📄 许可 (License)

本项目遵循 **MIT License** 开源协议。

## 📧 联系方式 (Contact)

Email: JontyLi0312@outlook.com