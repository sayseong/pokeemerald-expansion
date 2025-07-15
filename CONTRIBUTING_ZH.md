# 为 pokeemerald-expansion 做贡献

首先，感谢您帮助改进 `pokeemerald-expansion`！❤️

我们鼓励并珍视所有贡献。请务必在贡献前阅读相关章节！这将使您和维护者的工作更轻松。我们期待看到您的贡献。🎉

## 错误报告

我们使用 [GitHub Issues](https://github.com/rh-hideout/pokeemerald-expansion/issues?q=sort%3Aupdated-desc+is%3Aissue+is%3Aopen+label%3Abug) 跟踪错误。

### 提交错误报告前应做什么？

- 该错误是否在最新未修改的 [`upcoming`](https://github.com/rh-hideout/pokeemerald-expansion/tree/upcoming) 或 [`master`](https://github.com/rh-hideout/pokeemerald-expansion/tree/master) 分支上重现？如不能，请勿提交报告——问题很可能由您的游戏修改引起
- 是否已有其他人发现此问题？请搜索[错误跟踪器](https://github.com/rh-hideout/pokeemerald-expansion/issues?q=label%3Abug)确认。若已有相关 issue，可在现有 issue 中补充信息

### 如何提交错误报告？

如果在项目中遇到问题，请新建一个 [issue](https://github.com/rh-hideout/pokeemerald-expansion/issues/new)

优质错误报告应包含充足信息，避免后续反复沟通。请尽可能完成以下检查清单：

- [ ] 清晰描述问题现象
- [ ] 提供重现步骤（如："1. 打开菜单 2. 选择道具 3. 发生崩溃"）
- [ ] 说明预期行为与实际行为的差异
- [ ] 附上相关截图/视频（如有）
- [ ] 注明使用的游戏版本和分支

### 提交错误报告后会发生什么？

- 维护者将添加[标签](https://github.com/rh-hideout/pokeemerald-expansion/labels)
- 维护者将尝试按您提供的步骤复现错误
  - 若无重现步骤或无法复现，将标注为 `bug:unconfirmed`（未确认错误）
- 成功复现后将标注为 `bug:confirmed`（已确认错误），等待[修复](#拉取请求)
  - 严重问题将加入版本[里程碑](https://github.com/rh-hideout/pokeemerald-expansion/milestones)，该版本发布前必须修复

## 功能请求

本节指导您如何提交功能请求（包括全新功能和现有功能改进）。遵循指南将帮助维护者和社区理解您的建议。

- 我们使用 [GitHub Issues](https://github.com/rh-hideout/pokeemerald-expansion/issues?q=sort%3Aupdated-desc+is%3Aissue+is%3Aopen+label%3Afeature-request) 跟踪功能请求

### 提交功能请求前应做什么？

- 确认请求在[pokeemerald-expansion 范畴](docs/team_procedures/scope.md)内
- 若不明确是否在范畴内，请在 [RHH Discord 服务器](https://discord.gg/6CzjAG6GZk)的 [#pr-discussions](https://discord.com/channels/419213663107416084/1102784418369785948) 频道发起讨论

### 如何提交功能请求？

向项目添加功能请求时，请新建 [feature request](https://github.com/rh-hideout/pokeemerald-expansion/issues/new)

请包含以下信息：
- [ ] 功能解决的痛点
- [ ] 建议的解决方案
- [ ] 替代方案（如有）
- [ ] 附加上下文（如截图/概念设计）

### 提交功能请求后会发生什么？

- 维护者将添加[标签](https://github.com/rh-hideout/pokeemerald-expansion/labels)
- 若超出[范畴](docs/team_procedures/scope.md)，将关闭该请求
- 若在范畴内，其他贡献者可自愿[通过拉取请求实现](#拉取请求)。实现后请求将关闭

## 拉取请求

若阅读后仍有疑问，可在 Discord 的 #pr-discussions 频道发起讨论，或在 #expansion-dev 频道提问

### 提交拉取请求前应做什么？

- **Git 新手**：参考 [Team Aqua 资源库](https://github.com/Pawkkie/Team-Aquas-Asset-Repo/)的[fork与clone指南](https://github.com/Pawkkie/Team-Aquas-Asset-Repo/wiki/The-Basics-of-GitHub)，确保拥有[pokeemerald-expansion 本地副本](INSTALL.md)
- 确认贡献在[范畴](docs/team_procedures/scope.md)内（若不明确，请在 [RHH Discord](https://discord.gg/6CzjAG6GZk) 的 #pr-discussions 频道讨论）
- **选择目标分支**：
  - **`master`**：修复当前 `master` 分支的错误
  - **`upcoming`**：其他所有拉取请求
- 基于所选分支的最新版本创建新分支
- 若贡献涉及大量代码变更，建议先联系维护者确认审核意向（参见[维护者列表](#维护者)）

### 如何提交拉取请求？

#### 1. 获取本地工作副本
按 [INSTALL.md](INSTALL.md) 获取 `pokeemerald-expansion` 本地工作副本

#### 2. 添加 RHH 远程仓库
将主仓库设为远程源：
```bash
git remote add RHH https://github.com/rh-hideout/pokeemerald-expansion # RHH 可替换为任意名称
```

#### 3. 创建新分支

创建并切换到新分支：

```bash
git switch -c newFeature # 分支名可自定义
```

#### 4. 同步目标分支

使新分支与目标分支最新版本一致：

```bash
git reset --hard upcoming # 若目标为 master 分支则替换为 master
```

#### 5. 实现代码

所有工作应在新的干净分支上完成。若已有其他分支的工作，可通过 [cherry-pick](https://git-scm.com/docs/git-cherry-pick) 迁移提交，或直接复制文件变更

##### 社区功能分支注意事项

若基于已知社区功能分支开发，**强烈建议**提前发起讨论。某些情况下维护者会要求：

- 直接以现有功能分支为基础
- 或要求完全重写该功能

#### 6. 推送变更

首次提交需推送新分支到远程仓库：

```bash
git push --set-upstream origin newFeature
```

#### 7. 发起拉取请求

代码完成并推送后，请[从分支发起拉取请求](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request-from-a-fork)，目标选择 `pokeemerald-expansion` 的对应分支。**请完整填写 PR 描述**

### 提交拉取请求后会发生什么？

- 维护者将分配自己为审核者，通过 PR 审查提供反馈
- 贡献者需响应反馈并更新分支
- 贡献者需确保分支始终通过[合并检查清单](https://docs/team_procedures/merge_checklist.md)
- **重要**：维护者开始审核后，请勿使用 `force-push`（普通推送不受影响）
- 满足检查清单所有条件后，分支将被合并

## 维护者

最后更新：2025年4月1日

| 姓名                                        | Discord     | 当前活跃 | 专长领域                                                     |
| :------------------------------------------ | :---------- | :------- | :----------------------------------------------------------- |
| [Alex](https://github.com/AlexOn1ine)       | rainonline  | ✅        | 战斗引擎、战斗 AI                                            |
| [Egg](https://github.com/DizzyEggg)         | egg9255     | ✅        | 战斗引擎、战斗 AI                                            |
| [ghoulslash](https://github.com/ghoulslash) | ghoulslash  | ✅        | 图鉴导航、大地图、战斗引擎                                   |
| [Jasper](https://github.com/Bassoonian)     | bassoonian  | ✅        | 树果、昼夜系统、跟随系统、功能分支                           |
| [MGriffin](https://github.com/mrgriffin)    | mgriffin    | ✅        | 测试、训练家控制                                             |
| [psf](https://github.com/pkmnsnfrn)         | pkmnsnfrn   | ✅        | 再战系统、难度调节、训练家幻灯片、模拟实时时钟、钓鱼小游戏、英制/公制单位、大地图道具球、空战 |
| [Hedara](https://github.com/hedara90)       | hedara      | ✅        | 压缩算法、精灵图像                                           |
| [Pawkkie](https://github.com/Pawkkie)       | pawkkie     | ✅        | 战斗 AI                                                      |
| [SBird](https://github.com/SBird1337)       | karathan    | ✅        | 动态多选项、伤害计算、动画、训练家控制、测试                 |
| [Agustin](https://github.com/AgustinGDLV)   | agustingdlv | 未活跃   | 特殊机制、战斗引擎、测试、道具系统                           |
| [tertu](https://github.com/tertu-m)         | tertu       | 未活跃   | 随机化系统                                                   |

## 鸣谢

本指南基于 [contributing.md](https://contributing.md/generator) 创作！
