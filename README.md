# ATfE_test

一个关于 ATfE 编译器的测试项目，由 CubeMx 生成的基于 CMake 以及 arm-none-eabi 的项目移植而来。 \
如果在使用过程中发现任何问题，欢迎提交 issue 或者使用 email 联系我以改进。\
主控为 STM32H743IIT6，显示设备为 RGB 接口的 TFT 屏幕，相关引脚请通过工程下的 CubeMx 文件查看，工程默认使用 CMSIS-Dap 进行烧录

## 使用方法

本工程使用的工具有：CMake、Ninja、Openocd(optional)、Clang+llvm \

1. CMake: 构建系统生成器，用于管理项目构建过程、抽象构建细节、生成原生构建文件、处理交叉编译;
2. Ninja: 构建系统生成器，相比 make 可以更快解析输入文件;
3. Openocd: 开源调试、烧录软件;
4. Clang+llvm: LSP、提供函数跳转等功能;

编译器为：ATfE(Arm Toolchain for Embeded)，Github 仓库地址：<https://github.com/arm/arm-toolchain>
在仓库页面可找到 ATfE 的 README。

在使用前务必确定已经安装上述工具以及编译器，同时，还需要修改 cmake/ATfE.cmake 文件中的 ATfE 路径。

关于编译、flash 方法请参考 .vscode/tasks.json，可在 VS Code 上按 Ctrl-Shift-b 唤出任务列表执行。 \
建议执行顺序为

1. CMake Clean Build: 清除已经构建的文件
2. CMake Configure: 配置 CMake
3. CMake Build: 构建工程
4. Flash: 烧录，如果需要更改烧录器，请修改 tasks.json 下的 Flash 任务

## 联系方式

<ljt20030312@Outlook.com>
