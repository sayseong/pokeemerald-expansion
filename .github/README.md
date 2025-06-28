# 关于pokeemerald-expansion-Chinese

&zwnj;**`pokeemerald-expansion-Chinese`**&zwnj; 是基于ROM Hacking Hideout的[`pokeemerald-expansion`](https://github.com/rh-hideout/pokeemerald-expansion)项目的中文汉化项目。该项目的详细说明请见[关于`pokeemerald-expansion`](#关于`pokeemerald-expansion`)。

**本项目主要面向使用反编译方式进行改版的开发者，并非对于原作宝可梦绿宝石的汉化，也并不是一个可以独立游玩的宝可梦游戏。**

汉化内容包括：

- 参照2011年发布的由漫游&TGB联合汉化的口袋妖怪绿宝石汉化了游戏内的所有文本及文本图片，修改了部分与汉化后文字展示相关的代码。
- 汉化了项目内绿宝石原有的及expansion新增的全部的宝可梦、道具、特性、招式、属性、性格、异常状态等资料内容，译名参照目前官方正作简体中文的译名，并对提及了以上内容的所有游戏内文本进行了统一修正。
- 重新汉化了游戏内的大量文本，包括但不限于：城镇等地点名称、重要角色及普通NPC训练家的名称及所有对话，主线剧情及功能性NPC相关对话，华丽大赛、对战开拓区、联机俱乐部等设施的说明及其他文本，开拓区徒弟NPC、电视节目以及对战寻呼文本，以及例如“训练家”、“招式”、“培育屋”等游戏系列的专有名词文本。译名绝大部分参照目前官方正作简体中文及神奇宝贝百科的译名，并对提及了以上内容的所有游戏内文本进行了统一修正。
- 优化原本汉化版中的大量文本，包括但不限于：修正原汉化文本单行字数溢出换行的问题，换行符数量超标导致的文字显示不全的问题，统一游戏内文本的译名及标点使用，规范游戏内阿拉伯数字及汉字数字的使用，并对对话文本换行位置不当影响观感的情况进行了一定优化。
- 汉化了expansion项目新增的包括对战系统更新、图鉴、宝可梦跟随、测试debug等各项新增功能的全部相关文本，其中对战系统内的文本严格参照目前官方正作简体中文的翻译。
- 对于游戏内的easychat系统进行了汉化，对词库内短语进行了汉化并进行了一定程度的替换，对对战开拓区等使用easychat系统的NPC训练家的对话采用短语重拼的方式进行了汉化。
- 重新制作了标题logo，按照绿宝石日文版还原了一部分包括标题画面、读取存档及部分细节功能的内容。

注：本项目不包含对于expansion项目的优化及新功能开发。项目内的所有汉化文本可供宝可梦正作及改版汉化参考及使用，欢迎注明出处。

汉化人员名单（不分先后）：

- 字库破解及文本提取程序：卧看微尘，泡泡
- 文本整理、英文翻译及润色：Chaotix，sayseong，会飞的鱼
- 图片文字汉化及调整：兔砸博士，SKT，猫猫，Chaotix
- 日文翻译及日版功能还原：SKT
- 程序支持：明雅，墟芜，卧看微尘
- 测试BUG反馈：摆烂
- 标题LOGO绘制：大神丶橡皮

感谢大家的努力与贡献！

也感谢之前于2011年发布的漫游&TGB联合汉化的绿宝石汉化版的参与人员： 飞眼，enler，胡里胡涂，ルナ，零度雨，灰米，绿叶枫，yeyezai，lazybone，雪山肥壶，信天翁，爱姿病，troy1229，夜露死苦，阿宝，lioyy，凛，酷酷的安仔，空白君，AISU，吾鱼（小黑屋）

------

# 关于`pokeemerald-expansion`

![Gif that shows debugging functionality that is unique to pokeemerald-expansion such as rerolling Trainer Id, Cheat Start, PC from Debug Menu, Debug PC Fill, Pokemon Sprite Visualizer, Debug Warp to Map, and Battle Debug Menu](https://github.com/user-attachments/assets/cf9dfbee-4c6b-4bca-8e0a-07f116ef891c) ![Gif that shows overworld functionality that is unique to pokeemerald-expansion such as indoor running, BW2 style map popups, overworld followers, DNA Splicers, Gen 1 style fishing, OW Item descriptions, Quick Run from Battle, Use Last Ball, Wild Double Battles, and Catch from EXP](https://github.com/user-attachments/assets/383af243-0904-4d41-bced-721492fbc48e) ![Gif that shows off a number of modern Pokemon battle mechanics happening in the pokeemerald-expansion engine: 2 vs 1 battles, modern Pokemon, items, moves, abilities, fully customizable opponents and partners, Trainer Slides, and generational gimmicks](https://github.com/user-attachments/assets/50c576bc-415e-4d66-a38f-ad712f3316be)

&zwnj;**`pokeemerald-expansion`**&zwnj; 是一个GBA ROM改版蓝本，它为开发者提供了一个全面的工具包，用于创建宝可梦ROM改版。&zwnj;**`pokeemerald-expansion`**&zwnj; 是基于[pret的`pokeemerald`](https://github.com/pret/pokeemerald)反编译项目构建的。&zwnj;**它本身并不是一个可以独立游玩的宝可梦游戏。**&zwnj;

# [功能](FEATURES.md)

&zwnj;**`pokeemerald-expansion`**&zwnj; 提供了来自各种[宝可梦正作游戏系列](https://bulbapedia.bulbagarden.net/wiki/Core_series)的数百种功能，以及旨在简化开发流程并提升玩家体验的热门生活质量改进。这些功能的完整列表请见[`FEATURES_ZH.md`](FEATURES_ZH.md)。

# [致谢](CREDITS.md)

 [![](https://img.shields.io/github/all-contributors/rh-hideout/pokeemerald-expansion/upcoming)](CREDITS.md)

如果您使用了&zwnj;**`pokeemerald-expansion`**&zwnj;，请注明出处为&zwnj;**RHH (Rom Hacking Hideout)**&zwnj;。可以选择额外注明版本号。

```
基于RHH的pokeemerald-expansion 1.12.0 https://github.com/rh-hideout/pokeemerald-expansion/
```

请考虑[致谢](CREDITS.md)项目中的所有贡献者！

# 选择`pokeemerald` 还是 **`pokeemerald-expansion`**

- &zwnj;**`pokeemerald-expansion`**&zwnj; 支持与其他基于 &zwnj;**`pokeemerald-expansion`**&zwnj; 构建的游戏进行多人游戏功能。它与官方宝可梦游戏不兼容。
- 如果重视与官方游戏的兼容性，请使用[`pokeemerald`](https://github.com/pret/pokeemerald)作为蓝本。否则，我们建议使用 &zwnj;**`pokeemerald-expansion`**&zwnj;。
- &zwnj;**`pokeemerald-expansion`**&zwnj; 整合了来自`pokeemerald`的定期更新，包括错误修复和文档改进。

# [开始](INSTALL.md)

❗❗ &zwnj;**重要提示**&zwnj;：不要使用GitHub的“Download Zip”选项，因为它不包含提交历史。如果你想更新或合并其他功能分支，这是必不可少的。

如果你是git和GitHub的新手，[Team Aqua的资源仓库](https://github.com/Pawkkie/Team-Aquas-Asset-Repo/)提供了一个[关于fork和clone仓库的指南](https://github.com/Pawkkie/Team-Aquas-Asset-Repo/wiki/The-Basics-of-GitHub)。然后，你可以按照以下指南之一进行操作：

## 📥 [安装**`pokeemerald-expansion`**](INSTALL_ZH.md)
## 🏗️ [构建**`pokeemerald-expansion`**](INSTALL_ZH.md#构建 pokeemerald-expansion)
## 🚚 [从**`pokeemerald`**迁移](INSTALL_ZH.md#从 pokeemerald 迁移)

## 🚀 [升级**`pokeemerald-expansion`**](INSTALL_ZH.md#更新 pokeemerald-expansion)

# [文档](https://rh-hideout.github.io/pokeemerald-expansion/)

详细说明文档，请见[pokeemerald-expansion文档页](https://rh-hideout.github.io/pokeemerald-expansion/)。

# [贡献](CONTRIBUTING.md)
如果您想要[报告bug](CONTRIBUTING.md#Bug-Report)，[新建拉取请求](CONTRIBUTING.md#Pull-Requests)，或者[请求新功能](CONTRIBUTING.md#Feature-Request)，我们的[`CONTRIBUTING_ZH.md`](CONTRIBUTING_ZH.md)文件中有针对每一项的指南。

# [社区](https://discord.gg/6CzjAG6GZk)

[![](https://dcbadge.limes.pink/api/server/6CzjAG6GZk)](https://discord.gg/6CzjAG6GZk)

我们的社区使用[Rom Hacking Hideout (RHH) Discord服务器](https://discord.gg/6CzjAG6GZk)来进行沟通和组织活动。我们的大部分讨论都在那里进行，欢迎任何人加入我们！
