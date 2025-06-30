# 安装说明

各支持操作系统的安装说明可在 `docs/install/` 下相应目录中找到。
各系统对应的链接见下方标题。
本文档仅包含对各支持系统的简要介绍。
如遇问题，请在 Discord 寻求帮助（参见 [README_ZH.md](README_ZH.md)）。

完成适用于您操作系统的安装说明后，请继续阅读[构建 pokeemerald-expansion](#构建-pokeemerald-expansion)。

## Windows

**Windows 需要以下任一系统来构建项目**

**注意**：由于 Windows 7 和 Windows 8 已被微软官方停止支持，部分维护者可能不会长期维护 Win7/8 的安装说明。因此这些说明未来可能失效，修复时间可能长于 Windows 10/11 的说明修复。

在 Windows 上，可使用以下系统构建项目：

- WSL2（速度最快）
- WSL1（比 WSL2 慢 7 倍）
- Msys2（比 WSL2 慢 20 倍）（**注意**：目前在 pret 上游已损坏）
- Cygwin（比 WSL2 慢 30 倍）（**注意**：目前在 pret 上游已损坏）

**注意**：仅推荐使用 WSL 系统。

[WSL 安装说明](docs/install/windows/WSL_ZH.md)

[Msys2 安装说明](docs/install/windows/MSYS2_ZH.md)

[Cygwin 安装说明](docs/install/windows/CYGWIN_ZH.md)

## Linux

该项目可在任意 Linux 发行版上构建。
已有说明的发行版：

- [Ubuntu](docs/install/linux/UBUNTU_ZH.md)
- [Debian](docs/install/linux/DEBIAN_ZH.md)
- [Arch Linux](docs/install/linux/ARCH_LINUX_ZH.md)
- [NixOS](docs/install/linux/NIXOS_ZH.md)

其他发行版请参考 [通用说明](docs/install/linux/OTHERS.md)。

## Mac

需要额外配置才能使测试系统正常工作。

[Mac 安装说明](docs/install/mac/MAC_OS_ZH.md)

## ChromeOS

仅在 x86_64 架构系统上测试过。

[Chrome OS 安装说明](docs/install/chromeos/CHROME_OS_ZH.md)

# 构建 pokeemerald-expansion

按照以下步骤构建 `pokeemerald-expansion`：

1. 导航到您要存放项目的目录，注意任何系统特定的限制。
2. 使用 `git` 下载 `pokeemerald-expansion`

```console
git clone https://github.com/rh-hideout/pokeemerald-expansion
```

3. 进入新下载的项目目录。

```
cd pokeemerald-expansion
```

4. 构建项目。

```
make
```

5. 如果一切正常，您将看到类似以下输出：

```
arm-none-eabi-ld: warning: ../../pokeemerald.elf has a LOAD segment with RWX permissions
Memory region         Used Size  Region Size  %age Used
           EWRAM:      243354 B       256 KB     92.83%
           IWRAM:       30492 B        32 KB     93.05%
             ROM:    26072244 B        32 MB     77.70%
cd build/modern && arm-none-eabi-ld  -T ../../ld_script_modern.ld --print-memory-usage -o ../../pokeemerald.elf <objs> <libs> | cat
tools/gbafix/gbafix pokeemerald.elf -t"POKEMON EMER" -cBPEE -m01 -r0 --silent
arm-none-eabi-objcopy -O binary pokeemerald.elf pokeemerald.gba
tools/gbafix/gbafix pokeemerald.gba -p --silent
```

构建完成的 ROM 将以 `pokeemerald.gba` 文件名位于项目目录中。

# 构建指南

## 并行构建

更多信息请参阅 [GNU 文档](https://www.gnu.org/software/make/manual/html_node/Parallel.html) 和 [Stack Exchange 讨论](https://unix.stackexchange.com/questions/208568)。

要加速构建，首先通过以下命令获取 `nproc` 值：

```
nproc
```

然后通过以下命令加速构建：

```
make -j<nproc 输出值>
```

将 `<nproc 输出值>` 替换为 `nproc` 命令返回的数字。

macOS 上无 `nproc` 命令。替代命令是 `sysctl -n hw.ncpu`（相关 [Stack Overflow 讨论](https://stackoverflow.com/questions/1715580)）。

### 其他工具链

要使用 devkitARM 以外的工具链构建，需用工具链路径覆盖 `TOOLCHAIN` 环境变量（该路径必须包含 `bin` 子目录）：

```
make TOOLCHAIN="/path/to/toolchain/here"
```

示例如下：

```
make TOOLCHAIN="/usr/local/arm-none-eabi"
```

要使用此工具链编译 `modern` 目标，还需存在 `lib`、`include` 和 `arm-none-eabi` 子目录。

### 构建调试信息

要在现代工具链下构建带调试符号和调试优化级别的 **pokeemerald.elf**：

```
make debug
```

# 选择分支

**pokeemerald-expansion** 提供不同分支供用户选择使用。

## 最新补丁版 (Latest Patch)

此选项包含所有官方发布的扩展功能和错误修复。

## `master` 分支

`master` 分支包含"最新补丁版"的所有功能，以及自该版本发布以来发现的所有错误修复。

## `upcoming` 分支

`upcoming` 分支包含"最新补丁版"的所有功能，以及自该版本发布以来添加的所有新功能。

`master` 分支的错误修复会不定期合并到 `upcoming`，但无固定时间表。

# 从 pokeemerald 迁移

1. 添加 RHH 作为 git 远程仓库

```
git remote add RHH https://github.com/rh-hideout/pokeemerald-expansion
```

1. 拉取您选择的分支
   有三种可选分支：

```
git pull RHH master # 如选择 upcoming 分支，将 master 替换为 upcoming
# 如选择最新补丁版，将 master 替换为 expansion
# 如选择特定版本（如 1.11.0），将 master 替换为 expansion/1.11.0
```

如果您未使用最新版 pret/pokeemerald，可能需要解决一些合并冲突。完成后，您即可使用 **pokeemerald-expansion**。

# 更新 pokeemerald-expansion

1. 添加 RHH 作为 git 远程仓库（若未添加）

```
git remote add RHH https://github.com/rh-hideout/pokeemerald-expansion
```

2. 检查当前版本
   本地 更新日志 会显示您仓库的当前版本。
3. 选择目标版本
   我们建议按以下顺序逐步更新到下一版本：
   若当前版本低于 1.6.2，应先更新到 1.6.2
   - 1.6.2
   - 1.7.4
   - 1.8.3
   - 1.9.4
   - 1.10.3

例如，若当前版本为 1.7.0，应更新到 1.7.4。

4. 拉取目标版本

```
git pull RHH expansion/X.Y.Z # 将 X.Y.Z 替换为目标版本，如 `1.9.3`、`master` 或 `upcoming`
```

可能需要解决合并冲突。

如果您选择的目标版本不是 [标签页](https://github.com/rh-hideout/pokeemerald-expansion/tags) 上的最新版本，请重复步骤 3 和 4 直至更新到最新版。

# 实用附加工具

- [porymap](https://github.com/huderlem/porymap) - 查看和编辑地图
- [porytiles](https://github.com/gruntlucas/porytiles) - 为地图添加新元图块
- [poryscript](https://github.com/huderlem/poryscript) - 脚本编写（[VS Code 扩展](https://marketplace.visualstudio.com/items?itemName=karathan.poryscript)）
- [Tilemap Studio](https://github.com/Rangi42/tilemap-studio) - 查看和编辑图块地图
