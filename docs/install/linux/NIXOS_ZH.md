# NixOS编译指南
运行以下命令以启动一个包含必要包的交互式 shell：
```bash
nix-shell -p pkgsCross.arm-embedded.stdenv.cc git pkg-config libpng
```
