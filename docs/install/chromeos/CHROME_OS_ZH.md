# ChromeOS编译指南

1. 按照[此页面](https://chromeos.dev/en/productivity/terminal)上的说明启用Linux终端。请确保为Linux安装分配足够的空间。

2. Linux终端安装完成后，在终端中运行以下命令以更新和升级Linux终端：

    ```console
    sudo apt update && apt upgrade
    ```
    
3. 然后，通过运行以下命令安装所有依赖项：

    ```console
    sudo apt install build-essential binutils-arm-none-eabi gcc-arm-none-eabi libnewlib-arm-none-eabi git libpng-dev python3
    ```
    ‌**注意**‌：项目必须保存在Linux文件系统中的某个目录下，例如`~/Decomps/pokeemerald-expansion`下。

    
