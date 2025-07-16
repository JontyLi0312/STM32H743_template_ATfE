# ATfE_test

基于 ATfE 编译器的 STM32H743IIT6 测试项目，源文件由 CubeMX 生成的 CMake、starm-clang 的项目移植而来。 \
由于学业问题，并未进行全方位测试（例如 RTOS 下的使用情况），如果在使用过程中发现任何问题，欢迎提交 issue 或通过 email 联系。\
主控为 STM32H743IIT6，使用的是反客科技的最小系统板，显示设备为 RGB 接口的 TFT 屏幕，相关引脚请通过工程下的 CubeMx 文件查看，工程默认使用 CMSIS-Dap 进行烧录。

## 工具链

此工程使用的工具有：CMake、Ninja、OpenOCD、Clang+llvm。 \

1. CMake: 构建系统生成器，用于管理项目构建过程、抽象构建细节、生成原生构建文件、处理交叉编译;
2. Ninja: 构建系统生成器，相比 make 可以更快解析输入文件;
3. OpenOCD: 开源调试、烧录软件;
4. Clang+llvm: LSP、提供函数跳转等功能;

使用的编译器为：ATfE(Arm Toolchain for Embeded)，Github 仓库地址：<https://github.com/arm/arm-toolchain>
在仓库页面可找到 ATfE 的 README。

在使用前务必确定已经安装上述工具以及编译器，同时，还需要修改 cmake/ATfE.cmake 文件中的 ATfE 路径。

## 使用方法

关于构建、flash 指令请参考 .vscode/tasks.json。 \

如果对 CMake 构建项目不熟悉，建议执行步骤为(可在 VS Code 上按 Ctrl-Shift-b 唤出默认任务列表)：
1. 🧹 Debug: Clean -- 清除已经构建的文件，clone 工程后首次执行可能会报错(可忽略)，因为当前并没有 build 目录；
3. 🔨 Debug: Build -- 构建、编译工程（此任务依赖于 🔧 Debug: Configure，执行时会自动执行 configure）；
4. 📥 Debug: Flash -- 烧录，如果需要更改烧录器或烧录工具，请修改 tasks.json 下的 Flash 任务。
5. 也可以使用组合任务 🚀 Debug: Clean + Build + Flash，快速完成 Clean、build、flsh 操作。
上述步骤使用 Debug 预设，此工程提供了 Debug 预设以及 Release 预设，两者仅有优化等级的不同，前者为 g3，后者为 g0。
若要使用 Release 预设的任务，在 vscode 中按下 Ctrl+Shift+P 唤出命令面板，输入 Tasks: Run Task 来选择对应任务。

## 联系方式

<ljt20030312@Outlook.com>
