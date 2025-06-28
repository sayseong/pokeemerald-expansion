- # 包含哪些功能？

  ## 目录

  - [包含哪些功能？](#包含哪些功能)
    - [目录](#目录)
    - [配置文件](#配置文件)
    - [升级的战斗引擎](#升级的战斗引擎)
    - [完整的训练家自定义](#完整的训练家自定义)
    - [宝可梦数据](#宝可梦数据)
    - [界面改进](#界面改进)
    - [引擎改进](#引擎改进)
    - [大地图改进](#大地图改进)
    - [开发者工具](#开发者工具)

  ## 配置文件

  下面列出的许多功能都可以根据需要关闭。在这些文件中检查哪些功能可以配置：

  - [AI 配置](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/ai.h)
  - [战斗配置](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/battle.h)
  - [限制配置](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/caps.h)
  - [调试配置](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/debug.h)
  - [宝可梦导航系统 (DexNav) 配置](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/dexnav.h)
  - [通用配置](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/general.h)
  - [心金魂银 (HGSS) 图鉴配置](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/pokedex_plus_hgss.h)
  - [道具配置](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/item.h)
  - [NPC 跟随者配置](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/follower_npc.h)
  - [大地图配置](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/overworld.h)
  - [宝可梦配置](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/pokemon.h)
  - [存档配置](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/save.h)
  - [启用的宝可梦种类](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/species_enabled.h)
  - [摘要屏幕配置](https://github.com/rh-hideout-chinese/pokeemerald-expansion/blob/master/include/config/summary_screen.h)

  ## 升级的战斗引擎

  - ***战斗特殊机制 :*** 超级进化、原始回归、究极爆发、Z招式、极巨化、超极巨化和太晶化。
  - ***新世代战斗类型:*** 双打野生对战、自定义多人对战、反转对战、1v2/2v1 对战、空中对战。
  - ***更新的战斗机制:*** 高级球捕获、冻伤状态支持、精灵球快捷菜单、招式描述菜单、无徽章加成、第四世代雾天、听话机制、亲密度影响、捕获后队伍交换、战斗中显示效果绝佳/一般、《火红叶绿》/第四世代起战斗失败金钱计算、第四世代风格大地图人物影子。
  - ***更新的招式数据:*** 妖精/星晶属性、物理/特殊招式分类、各种招式标志。
  - ***更新的计算方式:*** 伤害、经验值、回合中速度计算、战斗结束后的状态和努力值、100级努力值计算。
  - ***截至第九世代的每个道具、特性和招式效果:*** 包括截至《剑盾》的华丽大赛数据 ([来源](https://pokemonurpg.com/info/contests/rse-move-list/))。
  - ***初始战斗条件:*** 能力等级、战场地形、野生宝可梦 AI 标志。
  - ***更快的战斗:*** 同时减少HP、快捷“逃跑”选项、更快的战斗开场、更快的HP减少动画、更快的AI计算。
  - ***更简单的自定义:*** 更简洁的代码库，便于实现自定义招式和效果。
  - ***改进的 AI:*** 更快，并考虑了扩展版添加的新效果。
  - ***受欢迎的功能:***等级/努力值上限、睡眠条款、属性指示器。

  ## 完整的训练家自定义

  - ***兼容 Pokémon Showdown 的队伍语法:*** 在[队伍构筑器](https://play.pokemonshowdown.com/teambuilder)中创建你的训练家队伍并粘贴结果！
  - ***自定义宝可梦数据:*** 昵称、努力值、个体值、招式、特性、精灵球、亲密度、性格、性别、闪光、极巨等级、超极巨化因子 和太晶属性。
    - ***“王牌宝可梦”:*** 会将特定宝可梦保留到最后出场。
    - ***训练家池:*** 一个训练家可以拥有一个随机宝可梦池，而不是固定的队伍。
  - ***自定义训练家切入对话*** 首回合、命中效果绝佳时、超级进化前等。
  - ***新的 AI 标志选项:*** 自定义训练家的智能程度。
  - ***训练家类别对应的精灵球:*** 潜水员使用潜水球、培育家使用巢穴球等。

  ## 宝可梦数据

  - ***改进的宝可梦数据结构:*** 优化了空间占用，允许容纳更多信息，如太晶属性、12字符名称、极限特训过的能力、进化条件、保存的HP/状态异常。
  - ***更新的培育机制:*** 精灵球/蛋招式/特性/性格的遗传、等级1蛋自动适用熏香宝宝规则。
  - ***更新的宝可梦种类数据:*** 种族值、属性、特性、隐藏特性、蛋群、努力值产出、招式学习列表、对战设施禁用、保证完美个体值数量、《欧米伽红宝石/阿尔法蓝宝石》图鉴编号。
  - ***更简单的宝可梦种类数据操作:*** 添加一个新宝可梦只需编辑约5个文件，而不是原版 pokeemerald 的20多个文件。
  - ***更新的精灵图:*** DS风格的精灵图，支持绿宝石的2帧动画和性别差异。
  - ***宝可梦种类开关:*** 可以禁用特定的宝可梦组别以节省空间，包括家族、跨世代进化、超级进化、地区形态等。
  - ***重构的进化系统:*** 多个进化条件可以堆叠，无需额外编码即可创建复杂的进化方式。支持除亲密度和主机陀螺仪外的所有条件。
  - ***形态变化系统:*** 大多数形态变化可以在不额外编码的情况下添加。这包括对以下机制的支持：持有/使用道具、达到HP阈值、战斗内外天气变化、融合等。

  ## 界面改进

  - ***宝可梦摘要:*** 招式回忆、查看努力值/个体值、性格能力加减颜色标识 ([功能分支](https://github.com/DizzyEggg/pokeemerald/tree/nature_color) by @DizzyEggg)。
  - ***队伍菜单:*** “移动道具”选项。
  - ***宝可梦存储系统:*** 置顶“移动”选项、可通过盒子链接道具访问。
  - ***心金魂银风格图鉴*** ([原始功能分支](https://github.com/TheXaman/pokeemerald/tree/tx_pokedexPlus_hgss) by @TheXaman): 玩家可访问的详细游戏内信息。

  ## 引擎改进

  - ***默认实现了所有基础 pokeemerald 的错误修复:*** 所有在 `BUGFIX` 定义下的修复。
  - ***改进的精灵图和调色板压缩:*** 资源比原版压缩占用更少空间。
  - ***现代编译器支持:*** 更容易检测代码中的潜在错误。
  - ***动态多选菜单*** ([原始分支](https://github.com/SBird1337/pokeemerald/tree/feature/dynmulti) by @SBird1337): 更简单的方式为脚本添加多选菜单。
  - ***高质量随机数生成器:*** 不再有原版破碎的 RNG。

  ## 大地图改进

  - ***现代机制:*** 清除浓雾场地技能、《黑2白2》+驱虫剂系统、室内奔跑、移除大地图中毒状态、连锁钓鱼、对战搜寻器、《火红叶绿》+战斗失败消息。
  - ***大地图行走图和跟随宝可梦*** ([功能分支](https://github.com/aarant/pokeemerald/tree/followers-expanded-id) by @aarant)
    - *包含动态大地图调色板 (DOWP) 和扩展大地图事件ID (支持超过255)。*
    - *包含截至第九世代的宝可梦精灵图。*
  - ***昼夜系统:*** ([功能分支](https://github.com/aarant/pokeemerald/tree/lighting-expanded-id) by @aarant)
    - *包含对非实时时钟的支持*。
  - ***NPC 跟随者:*** ([功能分支](https://github.com/ghoulslash/pokeemerald/tree/follow_me) by @ghoulslash)
  - ***黑白风格地图弹出框*** ([功能分支](https://github.com/ravepossum/pokeemerald/tree/bsbob_map_popups) by @BSBob)
  - ***XY 树果机制:*** 突变、湿度、杂草、害虫。
  - ***获得道具时显示说明*** (功能分支 by @ghoulslash)。

  ## 开发者工具

  - ***集成测试:*** 精确定位你的自定义机制是否破坏了游戏中的其他功能。
  - ***宝可梦精灵图查看器:*** 测试每个宝可梦的精灵图和动画。
  - ***大地图调试菜单*** ([原始功能分支](https://github.com/TheXaman/pokeemerald/tree/tx_debug_system) by @TheXaman): 支持菜单，包含一系列功能以方便调试，包括传送、开关标志和变量、生成宝可梦和道具等。
  - ***战斗调试菜单:*** 在战斗中途即时修改数据。
  - ***招式学习列表助手:*** 根据官方兼容性数据，从你的自定义TM和教学招式中自动生成招式学习列表。
  - ***可配置的脚本标志:*** 禁用野生遭遇、禁用训练家对战、强制/禁用闪光。
  - ***存档块清理*** ([功能分支](https://github.com/ghoulslash/pokeemerald/tree/saveblock) by @ghoulslash)
