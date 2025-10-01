# STM32H743IIT6 项目模板 (基于 ATfE)

[English](./README_en.md) | **中文**

一个为 STM32H743IIT6 打造的现代化项目模板，基于 Arm 官方嵌入式工具链 (ATfE)，旨在提供一个高效、纯净且易于扩展的模板。

## 📋 概述

本项目是一个基于 **Arm Toolchain for Embedded (ATfE)** 编译器的 STM32H743IIT6 项目模板。其初始化代码由 STM32CubeMX 生成，并已移植到 **CMake** + **Clang** 的现代化构建体系中，以带来更佳的开发体验。

### ✨ 特性

  * **现代化工具链**: 基于 Clang+LLVM 和 ATfE，告别传统工具链的限制。
  * **跨平台构建**: 使用 CMake 和 Ninja，可以在 Windows, macOS, Linux 上获得一致的构建体验。
  * **VS Code 集成**: 预置了详细的 `tasks.json` 和 `launch.json`，实现一键编译、烧录和调试。
  * **高度可定制**: 结构清晰，易于集成你自己的业务代码、中间件或 RTOS。
  * **DMA 内存优化**: 预先配置了独立的 RAM 区域 (`.buffer` 段) 用于 DMA，避免缓存一致性问题。

### 🔌 硬件平台

  * **开发板 (Board)**: 反客科技 (Fank-Tech) STM32H743IIT6 最小系统板
  * **显示屏 (Display)**: RGB 接口 TFT 屏幕
  * **引脚配置 (Pinout)**: 详见工程目录下的 `.ioc` 文件，可使用 STM32CubeMX 打开查看和修改。

-----

## 🚀 快速开始

下面将引导你完成开发环境的配置和项目的运行，强烈推荐使用 **VS Code**。

### 🛠️ 环境准备

请确保以下工具已正确安装，并已将其路径添加至系统 `PATH` 环境变量中。

| 工具 (Tool)                    | 说明 (Description)                            | 链接 (Link)                                                                                                           |
| :----------------------------- | :-------------------------------------------- | :-------------------------------------------------------------------------------------------------------------------- |
| **CMake**                      | 跨平台构建系统生成器                          | [cmake.org](https://cmake.org/)                                                                                       |
| **Ninja**                      | 高性能构建系统                                | [github.com/ninja-build/ninja](https://github.com/ninja-build/ninja)                                                  |
| **OpenOCD**                    | 开源片上调试和烧录工具                        | [github.com/openocd-org/openocd](https://github.com/openocd-org/openocd)                                              |
| **pyOCD**                      | 通过 USB 烧录和调试 MCU 的 Python 库          | [github.com/pyocd/pyOCD](https://github.com/pyocd/pyOCD)                                                              |
| **ATfE**                       | Arm 官方嵌入式工具链 (基于 LLVM)              | [arm.com/toolchain](https://www.google.com/search?q=https://developer.arm.com/downloads/-/arm-toolchain-for-embedded) |
| **GNU Arm Embedded Toolchain** | 提供 GDB 调试器                               | [arm.com/gnu-toolchain](https://developer.arm.com/downloads/-/gnu-rm)                                                 |
| **LLVM**                       | 提供 `clangd` 用于语言服务 (代码补全、跳转等) | [github.com/llvm/llvm-project](https://github.com/llvm/llvm-project)                                                  |

### 📦 VS Code 插件

请在 VS Code 插件市场中安装以下插件：

1.  **Cortex-Debug**: 用于 ARM Cortex-M 内核的调试。
2.  **Clangd**: 用于代码补全、语法检查和定义跳转。
3.  **CMake Tools**: 用于 CMake 的配置、构建和管理。

### ⚙️ 项目配置

1.  克隆本仓库到本地。
2.  打开 `cmake/ATfE.cmake` 文件。
3.  修改 `TOOLCHAIN_PREFIX` 变量，使其指向你的 **ATfE** 工具链的实际安装路径。

### 🏃‍♀️ 编译与烧录
本项目已预置了 VS Code 任务，以简化开发流程。使用快捷键 `Ctrl+Shift+B` 唤出任务面板，从列表中选择你想要执行的任务。

可用任务列表如下：

基础任务 (Basic Tasks): 执行单一操作的基本任务。

1.  **🔧 Configure**: 配置 CMake 环境。Build 任务会自动调用它，通常无需手动执行。
2.  **🔨 Build**: 编译项目（依赖于 🔧 Configure）。
3.  **🧹 Clean**: 清理所有构建生成的文件。
4.  **📥 Flash (OpenOCD)**: 使用 OpenOCD （默认为CMSIS-DAP）将固件烧录到目标设备。 
5.  **📥 Flash (pyOCD)**: 使用 pyOCD （任意受 pyOCD 支持的烧录器）将固件烧录到目标设备。

组合工作流 (Composite Workflows): 按顺序执行多个基础任务的便捷工作流。

1.  **🚀 Clean + Build**: 自动完成清理和编译两个步骤。

2. **🚀🚀 Clean + Build + Flash (OpenOCD)**: (推荐) 全自动工作流，依次执行清理、编译，并使用 OpenOCD 进行烧录。

3. **🚀🚀 Clean + Build + Flash (pyOCD)**: 全自动工作流，依次执行清理、编译，并使用 pyOCD 进行烧录。

所有任务的详细定义均可在 `.vscode/tasks.json` 文件中查看和修改。

-----

## 🔬 调试 (Debugging)

1.  **GDB 依赖**: 本项目的调试功能依赖 **GNU Arm Embedded Toolchain** 中的 GDB。请确保已安装该工具链并将其添加至系统 `PATH`。
2.  **调试配置**: 所有的调试配置都预设在 `.vscode/launch.json` 文件中。
3.  **启动调试**:
      * 在 VS Code 的侧边栏切换到“运行和调试”视图。
      * 根据你使用的调试器，从顶部的下拉菜单中选择 `openocd_CMSIS-DAP`、`openocd_ST-Link` 或 `pyOCD` 配置。
      * 按 `F5` 键启动调试会话。

> **注意**: `launch.json` 中也包含了 `probe-rs` 的配置项，但目前尚不稳定，不推荐用于调试。不过，使用 `probe-rs` 进行烧录经过验证后是可行的。

-----

## 🏗️ 构建配置

项目预设了两种构建类型，以满足不同开发阶段的需求：

| 配置 (Profile) | 优化等级 (Optimization) | 用途 (Usage)       |
| :------------- | :---------------------- | :----------------- |
| **Debug**      | `-g3`                   | 用于日常开发和调试 |
| **Release**    | `-g0`                   | 用于最终的产品发布 |

你可以通过 VS Code 的 CMake Tools 插件或底部状态栏轻松切换构建配置。

-----

## ⚠️ 重要注意事项：DMA 与缓存

为了解决 **DMA** 访问外设时的缓存一致性问题，我们在链接脚本 (`.ld` 文件) 中新增了一个位于 AXI SRAM (`0x24000000`) 的特殊内存段 `.buffer`。

当你需要定义用于 DMA 的缓冲区时，请使用以下属性将其放入指定内存区域：

```c
__attribute__((section(".buffer"))) __attribute__((aligned(4))) uint8_t my_dma_buffer[1024];
```

如果你启用了 D-Cache，在每次 CPU 修改了这块内存的数据后、启动 DMA 传输前，**必须**手动清理缓存，以确保外设能访问到最新的数据：

```c
SCB_CleanDCache_by_Addr((uint32_t*)my_dma_buffer, sizeof(my_dma_buffer));
```

-----

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

-----

## 🤝 贡献 (Contributing)

欢迎提交 Issue 或 Pull Request！如果你发现了任何问题或有改进建议，请随时提出。

## 📄 许可 (License)

本项目采用 **MIT** 许可。

## 📧 联系方式 (Contact)

Email: ljt20030312@outlook.com