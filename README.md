# STM32H743IIT6 Template for ATfE

基于 ATfE (Arm Toolchain for Embedded) 编译器的 STM32H743IIT6 项目模板

## 📋 概述

本项目是一个基于 ATfE 编译器的 STM32H743IIT6 测试模板，源文件由 STM32CubeMX 生成的 CMake + starm-clang 项目移植而来。

### 硬件平台
- **开发板**: 反客科技 STM32H743IIT6 最小系统板
- **显示屏**: RGB 接口 TFT 屏幕
- **引脚配置**: 详见工程目录下的 `.ioc` 文件

## ⚠️ 注意
使用串口 DMA 时，需要将传输的数据定义为全局变量，或者将传输的数据存放到 0x24000000 后的内存空间（参考 STM32H7 手册中 DMA 可访问的内存地址），否则串口 DMA 传输会失败。

本项目尚未进行全面测试（如 RTOS 环境下的使用），遇到问题欢迎提交 Issue 或通过邮件联系。

## 🛠️ 开发环境

### 必需工具

| 工具           | 说明                              | 链接                                             |
| -------------- | --------------------------------- | ------------------------------------------------ |
| **CMake**      | 跨平台构建系统生成器              | [cmake.org](https://cmake.org/)                  |
| **Ninja**      | 高性能构建系统                    | [GitHub](https://github.com/ninja-build/ninja)   |
| **OpenOCD**    | 开源调试和烧录工具                | [GitHub](https://github.com/openocd-org/openocd) |
| **Clang+LLVM** | 语言服务器和代码导航              | [GitHub](https://github.com/llvm/llvm-project)   |
| **ATfE**       | Arm 官方嵌入式工具链（基于 LLVM） | [GitHub](https://github.com/arm/arm-toolchain)   |

确保上述工具已经添加至 PATH 环境变量中，以保证 vscode 能正确调用。

### 环境配置

1. 安装上述所有工具并添加到 'PATH' 中；
2. 修改 `cmake/ATfE.cmake` 文件中 ATfE 路径。

## 🚀 快速开始

### VS Code 用户（推荐）

使用 `Ctrl+Shift+B` 打开任务列表，按顺序执行：

1. **🧹 Debug: Clean** - 清理构建文件
2. **🔨 Debug: Build** - 构建项目（自动执行 Configure）
3. **📥 Debug: Flash** - 烧录程序

或使用一键操作：
- **🚀 Debug: Clean + Build + Flash** - 一键完成所有步骤

### 构建配置

项目提供两种构建预设：

| 预设        | 优化等级 | 用途     |
| ----------- | -------- | -------- |
| **Debug**   | -g3      | 调试开发 |
| **Release** | -g0      | 生产部署 |

切换预设：`Ctrl+Shift+P` → 输入 `Tasks: Run Task` → 选择对应任务

## Debug

项目通过 OpenOCD 调试器进行调试，需要安装 Cortex-Debug 插件。
在调试页面可以选择使用 daplink 或者 stlink。
launch.json 配置中有 Probe-RS 的调试配置，但是无法正常工作，不推荐使用，但是使用 Probe-RS 进行烧录是可行的。

## 📁 项目结构

STM32H743IIT6_template_ATfE/ \
├── cmake/ # CMake 配置文件 \
│ └── ATfE.cmake # ATfE 工具链配置 \
├── .vscode/ # VS Code 配置 \
│ └── tasks.json # 构建任务定义 \
├── CMakeLists.txt # 主 CMake 文件 \
└── STM32H743IIT6.ioc # STM32CubeMX 配置

## 📧 联系方式

- **Email**: ljt20030312@outlook.com
