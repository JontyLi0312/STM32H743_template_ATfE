# STM32H743IIT6 项目模板 (基于 ATfE)

[English](./docs/README_en.md) | **中文**

一个为 STM32H743IIT6 打造的现代化项目模板，旨在提供一个高效且易于扩展的模板。

## 📋 概述

本项目基于 **Arm Toolchain for Embedded (ATfE)** 编译器，该编译器由 Arm 基于 LLVM 以及 picolibc 打造。其初始化代码由 STM32CubeMX 生成，并已移植到 **CMake** + **Clangd** 的现代化体系中，以带来更佳的开发体验。

### ✨ 特性

  * **现代化工具链**：基于 Clang+LLVM 和 ATfE，告别传统工具链的限制。
  * **跨平台构建**：使用 CMake 和 Ninja，可以在 Windows, macOS, Linux 上获得一致的构建体验。
  * **VS Code 集成**：预置了详细的 `tasks.json` 和 `launch.json`，实现一键编译、烧录和调试。
  * **高度可定制**：结构清晰，易于集成你自己的业务代码、中间件或 RTOS。
  * **DMA 内存优化**：预先配置了独立的 RAM 区域 (`.ram_section` 段) 用于 DMA，避免缓存一致性问题。

### 🔌 硬件平台

  * **开发板 (Board)**：反客科技 STM32H743IIT6 最小系统板。
  * **显示屏 (Display)**：RGB 接口 TFT 屏幕。
  * **引脚配置 (Pinout)**：详见工程目录下的 `.ioc` 文件，可使用 STM32CubeMX 打开查看和修改。

## 🚀 快速开始

下面将引导你完成开发环境的配置和项目的运行，默认使用 **Visual Studio Code** 编辑器。

### 🛠️ 环境准备

请确保以下工具已正确安装，并将其可执行文件路径添加至系统 `PATH` 环境变量中。如果不希望将工具添加至环境变量，只需要确保可以调用下述工具即可。

| 工具 (Tool)           | 说明 (Description)                            | 链接 (Link)                                                                                                 |
| :-------------------- | :-------------------------------------------- | :---------------------------------------------------------------------------------------------------------- |
| **CMake**             | 跨平台构建系统生成器                          | [cmake.org](https://cmake.org/)                                                                             |
| **Ninja**             | 高性能构建系统                                | [github.com/ninja-build/ninja](https://github.com/ninja-build/ninja)                                        |
| **OpenOCD**           | 开源片上调试和烧录工具                        | [github.com/openocd-org/openocd](https://github.com/openocd-org/openocd)                                    |
| **pyOCD**             | 通过 USB 烧录和调试 MCU 的 Python 库          | [github.com/pyocd/pyOCD](https://github.com/pyocd/pyOCD)                                                    |
| **ATfE**              | Arm 官方嵌入式工具链 (基于 LLVM)              | [arm.com/toolchain](https://github.com/arm/arm-toolchain/blob/arm-software/arm-software/embedded/README.md) |
| **Arm GNU Toolchain** | 提供 GDB                                      | [arm.com/gnu-toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)                  |
| **LLVM**              | 提供 `clangd` 用于语言服务 (代码补全、跳转等) | [github.com/llvm/llvm-project](https://github.com/llvm/llvm-project)                                        |

有关 ATfE 的更多细节，建议仔细阅读 ARM 官方关于 ATfEP 的文档：https://developer.arm.com/documentation/107976/21-1-1/?lang=en。

ATfEP 与 ATfE 有相同功能，但是 ATfEP 由 ARM 提供了更多企业方面的功能，两者或许有些许不同，因此无法保证该文档对于 ATfE 完全适用。

### 📦 VS Code 插件

1. **Cortex-Debug**：用于调试。
2. **Clangd**：用于代码补全、语法检查和定义跳转。
3. **CMake Tools**：用于 CMake 的配置和管理。

### ⚙️ 项目配置

1. 克隆仓库到本地。
2. 打开 `cmake/ATfE.cmake` 文件。
3. 修改 `TOOLCHAIN_PREFIX` 变量，使其指向你的 **ATfE** 实际安装路径。

### 🏃‍♀️ 编译与烧录任务

本项目已预置了 VS Code 任务，以简化开发流程。使用快捷键 `Ctrl+Shift+B` 唤出任务面板，从列表中选择你想要执行的任务。

可用任务列表如下：
基础任务（Basic Tasks）：执行单一操作的基本任务。
1. **🔧 Configure**：配置 CMake 环境。Build 任务会自动调用它，通常无需手动执行。
2. **🔨 Build**：编译项目（依赖于 🔧 Configure）。
3. **🧹 Clean**：清理所有构建生成的文件。
4. **📥 Flash (OpenOCD)**：使用 OpenOCD（CMSIS-DAP）将固件烧录到目标设备。 
5. **📥 Flash (pyOCD)**：使用 pyOCD （任意受 pyOCD 支持的烧录器）将固件烧录到目标设备。

组合工作流 (Composite Workflows): 按顺序执行多个基础任务的便捷工作流。
1. **🧹-> 🔨**: 按顺序自动执行清理和编译两个步骤。
2. **🔨-> 📥 (OpenOCD/pyOCD)**: 按顺序执行编译，并使用 OpenOCD 或 pyOCD 进行烧录。
3. **🧹-> 🔨 -> 📥 (OpenOCD/pyOCD)**: 按顺序执行清理、编译，并使用 OpenOCD 或 pyOCD 进行烧录。

所有任务的详细定义均可在 `.vscode/tasks.json` 文件中查看和修改。

### 🔨 编译产物

在顶层 CMakeLists.txt 中添加了如下命令，将 .elf 转换为 .hex，将 .elf 剥离成仅包含内部 flash 数据的 .elf，将 .elf 剥离成仅包含外部 flash 数据的 .bin。
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

### 📥 外部 Flash 烧录

将数据烧录至外部 flash 建议使用 pyOCD 结合根目录下的 pyocd_user.py 以及 .conf/*.flm 文件，烧录命令如下：
```
pyocd flash -t stm32h743iitx build/Debug/internal_flash.elf build/Debug/external_flash.bin@0x90000000
```

**注意：** 使用本项目提供的 .flm 下载算法前请确保 qspi 配置与本项目的相同，否则需要自行准备 .flm 文件。

## 🔬 调试 (Debugging)

1. **GDB 依赖**: 本项目的调试功能依赖 **Arm GNU Toolchain** 中的 GDB 工具。请确保已安装该工具链并将其添加至系统 `PATH`。
2. **调试配置**: 所有的调试配置都预设在 `.vscode/launch.json` 文件中。
3. **启动调试**:
   * 在 VS Code 的侧边栏切换到“运行和调试”视图。
   * 根据你使用的调试器，从顶部的下拉菜单中选择 `openocd_CMSIS-DAP`、`openocd_ST-Link` 或 `pyOCD` 配置。
   * 按 `F5` 键启动调试会话。

> **注意**: `launch.json` 中包含的 `probe-rs` 的配置项不可用，不过，使用 `probe-rs` 进行烧录是可行的。如果使用了外部 flash，此时 pyocd 调试不可用，请切换至 openocd 或者下述的 ozone。

### ozone 调试

ozone 是 Segger 推出的图形化调试器，可以使用 JLink 调试 STM32 项目，如果希望使用 CMSIS-DAP 等其他 Link，ozone 也提供了 GDB 的选项，通过 OpenOCD 以及 pyOCD 连接至 MCU 后，在 ozone 中可以通过 GDB 进行调试，该功能目前需要申请证书（URL: https://www.segger.com/purchase/licensing/license-request/）。

## 🏗️ 构建配置

项目预设了两种构建类型，以满足不同开发阶段的需求：
| 配置 (Profile) | 优化等级 (Optimization) | 用途 (Usage)       |
| :------------- | :---------------------- | :----------------- |
| **Debug**      | `-g3`                   | 用于日常开发和调试 |
| **Release**    | `-g0`                   | 用于最终的产品发布 |

## ⚠️ 重要注意事项：DMA 与缓存

为了解决 **DMA** 访问外设的问题，在链接脚本 (`.ld` 文件) 中新增了一个位于 AXI SRAM (`0x24000000`) 的特殊内存段 `.ram_section`。
当你需要定义用于 DMA 的区域时，请使用以下属性将其放入指定内存区域：
```c
__attribute__(section(".ram_section"))) __attribute__((aligned(32))) uint8_t my_dma_buffer[1024];
```
如果启用了 D-Cache，在每次 CPU 修改了这块内存的数据后、启动 DMA 传输前，**必须**手动清理缓存，以确保外设能访问到最新的数据：
```c
SCB_CleanDCache_by_Addr((uint32_t*)my_dma_buffer, sizeof(my_dma_buffer));
```

## 📁 项目结构

```
STM32H743IIT6_template_ATfE/
├── App/                # 应用程序和业务逻辑代码
├── Core/               # STM32CubeMX 生成的核心代码 (Inc, Src)
├── Drivers/            # STM32CubeMX 生成的 HAL 库和 CMSIS
├── User_Drivers/       # 用户自定义的驱动代码 (例如传感器、屏幕等)
├── cmake/              # CMake 配置文件
│   └── ATfE.cmake      # ATfE 工具链定义文件
├── .vscode/            # VS Code 编辑器配置
│   ├── launch.json     # 调试配置
│   └── tasks.json      # 构建任务定义
├── .gitignore          # Git 忽略文件
├── CMakeLists.txt      # 主 CMake 配置文件
└── STM32H743IIT6.ioc   # STM32CubeMX 项目文件
```

## 🤝 贡献 (Contributing)

欢迎提交 Issue 或 Pull Request！如果你发现了任何问题或有改进建议，请随时提出。

## 📄 许可 (License)

本项目采用 **MIT** 许可。

## 📧 联系方式 (Contact)

Email: ljt20030312@outlook.com