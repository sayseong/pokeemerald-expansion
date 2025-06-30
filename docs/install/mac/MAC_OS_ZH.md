# macOS 编译指南
1. 如果未安装 Xcode 命令行工具，请点击[此处](https://developer.apple.com/xcode/resources/)下载工具，打开终端并运行以下命令：

    ```bash
    xcode-select --install
    ```

2.  - 如果 **未安装 libpng**，请跳转到 [安装 libpng (macOS)](# 安装 libpng (macOS))
    - 如果 **未安装 pkg-config**，请跳转到 [安装 pkg-config (macOS)](#安装 pkg-config (macOS))
    - 如果 **未安装 devkitARM**，请跳转到 [安装 devkitARM (macOS)](#安装 devkitARM (macOS))
    - 如果以上都已安装，请**打开终端**并跳转到 [选择 pokeemerald-expansion 存储位置 (macOS)](#choosing-where-to-store-pokeemerald-expansion-macos)

3. **可选：运行测试**  
   如果未安装 Homebrew 环境，请参考[此指南](https://brew.sh)安装包管理器。打开终端运行：
    ```bash
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    brew install coreutils
    ```

4. **可选：通过 Rosetta 运行测试**
    - 通常不建议这样做（速度较慢）。多数用户可使用原生工具，但某些情况（如使用仅支持 Intel 的自定义工具链）可能需要此配置
    - 需安装 Intel 兼容的 Homebrew，配置方法参考[此说明](https://github.com/Homebrew/brew/issues/9173#issuecomment-729206868)
    - 使用 Intel 兼容的 Homebrew 安装 `coreutils`（步骤同第 3 步）

### 安装 libpng (macOS)
<details>
    <summary><i>高级用户注意...</i></summary>

>   本指南通过 Homebrew 安装 libpng（最简单方法），高级用户可通过其他方式自行安装

1. 打开终端
2. 如未安装 Homebrew，请按官网说明安装 [Homebrew](https://brew.sh/)
3. 运行以下命令安装 libpng：
    ```bash
    brew install libpng
    ```
    libpng 现已安装完成
    
    如果 **未安装 pkg-config** 请跳转至 [安装 pkg-config (macOS)](#安装 libpng (macOS))；如果 **未安装 devkitARM** 请跳转至 [安装 devkitARM (macOS)](#安装 devkitARM (macOS))
    
    若两者均已安装，请前往 [选择 pokeemerald-expansion 存储位置 (macOS)](#choosing-where-to-store-pokeemerald-expansion-macos)

### 安装 pkg-config (macOS)
<details>
    <summary><i>高级用户注意...</i></summary>

>   本指南通过 Homebrew 安装 pkg-config（最简单方法），高级用户可通过其他方式自行安装

1. 打开终端
2. 如未安装 Homebrew，请按官网说明安装 [Homebrew](https://brew.sh/)
3. 运行以下命令安装：
    ```bash
    brew install pkg-config
    ```
    pkg-config 现已安装完成
    
    如果 **未安装 devkitARM** 请跳转至 [安装 devkitARM (macOS)](#安装 devkitARM (macOS))，否则前往 [选择 pokeemerald-expansion 存储位置 (macOS)](#choosing-where-to-store-pokeemerald-expansion-macos)

### 安装 devkitARM (macOS)
1. 从[此链接](https://github.com/devkitPro/pacman/releases)下载 `devkitpro-pacman-installer.pkg` 安装包
2. 打开安装包完成 devkitPro pacman 的安装
3. 在终端运行以下命令安装 devkitARM：
    ```bash
    sudo dkp-pacman -Sy
    sudo dkp-pacman -S gba-dev
    sudo dkp-pacman -S devkitarm-rules
    ```
    gba-dev 安装时会提示选择软件包，直接按 Enter 键安装全部，然后输入 Y 确认安装

4. 安装完成后，需让系统全局访问 devkitARM。运行以下命令：
    ```bash
    export DEVKITPRO=/opt/devkitpro
    echo "export DEVKITPRO=$DEVKITPRO" >> ~/.zshrc
    export DEVKITARM=$DEVKITPRO/devkitARM
    echo "export DEVKITARM=$DEVKITARM" >> ~/.zshrc
    echo "if [ -f ~/.zshrc ]; then . ~/.zshrc; fi" >> ~/.zprofile
    ```
    *注：从 macOS 10.15 开始默认使用 zsh 终端。若从旧版本迁移，可能仍在使用 bash，可通过终端运行 `echo $0` 确认*
    <details>
        <summary><i>如果终端使用 bash 而非 zsh...</i></summary>

    ```bash
    export DEVKITPRO=/opt/devkitpro
    echo "export DEVKITPRO=$DEVKITPRO" >> ~/.bashrc
    export DEVKITARM=$DEVKITPRO/devkitARM
    echo "export DEVKITARM=$DEVKITARM" >> ~/.bashrc
    echo "if [ -f ~/.bashrc ]; then . ~/.bashrc; fi" >> ~/.bash_profile
    ```

### 安装 Python (macOS)
1. 从[官网](https://www.python.org/downloads/)下载最新版 Python 安装包
2. 打开安装包完成安装

Python 现已安装完成