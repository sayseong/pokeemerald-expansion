const u8 gEasyChatWord_Meets[] = _("遇见");
const u8 gEasyChatWord_Concede[] = _("答应");
const u8 gEasyChatWord_Give[] = _("给予");
const u8 gEasyChatWord_Gives[] = _("影响");
const u8 gEasyChatWord_Played[] = _("给了");
const u8 gEasyChatWord_Plays[] = _("玩耍");
const u8 gEasyChatWord_Collect[] = _("收集");
const u8 gEasyChatWord_Walking[] = _("走");
const u8 gEasyChatWord_Walks[] = _("弄");
const u8 gEasyChatWord_Says[] = _("行");
const u8 gEasyChatWord_Went[] = _("去");
const u8 gEasyChatWord_Said[] = _("说");
const u8 gEasyChatWord_WakeUp[] = _("最");
const u8 gEasyChatWord_WakesUp[] = _("醒来");
const u8 gEasyChatWord_Angers[] = _("激怒");
const u8 gEasyChatWord_Teach[] = _("教");
const u8 gEasyChatWord_Teaches[] = _("教授");
const u8 gEasyChatWord_Please[] = _("请");
const u8 gEasyChatWord_Learn[] = _("让");
const u8 gEasyChatWord_Change[] = _("改变");
const u8 gEasyChatWord_Story[] = _("故事");
const u8 gEasyChatWord_Trust[] = _("信任");
const u8 gEasyChatWord_Lavish[] = _("挥霍");
const u8 gEasyChatWord_Listens[] = _("坐");
const u8 gEasyChatWord_Hearing[] = _("听说");
const u8 gEasyChatWord_Trains[] = _("训练");
const u8 gEasyChatWord_Choose[] = _("选择");
const u8 gEasyChatWord_Come[] = _("来");
const u8 gEasyChatWord_Came[] = _("被");
const u8 gEasyChatWord_Search[] = _("搜寻");
const u8 gEasyChatWord_Make[] = _("制作");
const u8 gEasyChatWord_Cause[] = _("引起");
const u8 gEasyChatWord_Know[] = _("前进");
const u8 gEasyChatWord_Knows[] = _("知道");
const u8 gEasyChatWord_Refuse[] = _("拒绝");
const u8 gEasyChatWord_Stores[] = _("储存");
const u8 gEasyChatWord_Brag[] = _("吹嘘");
const u8 gEasyChatWord_Ignorant[] = _("无知");
const u8 gEasyChatWord_Thinks[] = _("认为");
const u8 gEasyChatWord_Believe[] = _("相信");
const u8 gEasyChatWord_Slide[] = _("滑行");
const u8 gEasyChatWord_Eats[] = _("用");
const u8 gEasyChatWord_Use[] = _("觉得");
const u8 gEasyChatWord_Uses[] = _("使用");
const u8 gEasyChatWord_Using[] = _("能");
const u8 gEasyChatWord_Couldnt[] = _("不能");
const u8 gEasyChatWord_Capable[] = _("阻止");
const u8 gEasyChatWord_Disappear[] = _("消失");
const u8 gEasyChatWord_Appear[] = _("无法");
const u8 gEasyChatWord_Throw[] = _("扔");
const u8 gEasyChatWord_Worry[] = _("担心");
const u8 gEasyChatWord_Slept[] = _("明白");
const u8 gEasyChatWord_Sleep[] = _("理解");
const u8 gEasyChatWord_Release[] = _("用力");
const u8 gEasyChatWord_Drinks[] = _("喝");
const u8 gEasyChatWord_Runs[] = _("跑");
const u8 gEasyChatWord_Run[] = _("冲刺");
const u8 gEasyChatWord_Works[] = _("留下");
const u8 gEasyChatWord_Working[] = _("工作");
const u8 gEasyChatWord_Talking[] = _("交谈");
const u8 gEasyChatWord_Talk[] = _("珍惜");
const u8 gEasyChatWord_Sink[] = _("沉没");
const u8 gEasyChatWord_Smack[] = _("拍打");
const u8 gEasyChatWord_Pretend[] = _("假装");
const u8 gEasyChatWord_Praise[] = _("赞扬");
const u8 gEasyChatWord_Overdo[] = _("展");
const u8 gEasyChatWord_Show[] = _("展示");
const u8 gEasyChatWord_Looks[] = _("看");
const u8 gEasyChatWord_Sees[] = _("看见");
const u8 gEasyChatWord_Seek[] = _("寻求");
const u8 gEasyChatWord_Own[] = _("拥有");
const u8 gEasyChatWord_Take[] = _("拿");
const u8 gEasyChatWord_Allow[] = _("允许");
const u8 gEasyChatWord_Forget[] = _("拿出");
const u8 gEasyChatWord_Forgets[] = _("忘记");
const u8 gEasyChatWord_Appears[] = _("出现");
const u8 gEasyChatWord_Faint[] = _("起来");
const u8 gEasyChatWord_Fainted[] = _("晕倒");

const struct EasyChatWordInfo gEasyChatGroup_Actions[] = {
    [EC_INDEX(EC_WORD_MEETS)] =
    {
        .text = gEasyChatWord_Meets,
        .alphabeticalOrder = EC_INDEX(EC_WORD_ALLOW),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CONCEDE)] =
    {
        .text = gEasyChatWord_Concede,
        .alphabeticalOrder = EC_INDEX(EC_WORD_ANGERS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GIVE)] =
    {
        .text = gEasyChatWord_Give,
        .alphabeticalOrder = EC_INDEX(EC_WORD_APPEAR),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_GIVES)] =
    {
        .text = gEasyChatWord_Gives,
        .alphabeticalOrder = EC_INDEX(EC_WORD_APPEARS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PLAYED)] =
    {
        .text = gEasyChatWord_Played,
        .alphabeticalOrder = EC_INDEX(EC_WORD_BELIEVE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PLAYS)] =
    {
        .text = gEasyChatWord_Plays,
        .alphabeticalOrder = EC_INDEX(EC_WORD_BRAG),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_COLLECT)] =
    {
        .text = gEasyChatWord_Collect,
        .alphabeticalOrder = EC_INDEX(EC_WORD_CAME),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WALKING)] =
    {
        .text = gEasyChatWord_Walking,
        .alphabeticalOrder = EC_INDEX(EC_WORD_CAPABLE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WALKS)] =
    {
        .text = gEasyChatWord_Walks,
        .alphabeticalOrder = EC_INDEX(EC_WORD_CAUSE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SAYS)] =
    {
        .text = gEasyChatWord_Says,
        .alphabeticalOrder = EC_INDEX(EC_WORD_CHANGE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WENT)] =
    {
        .text = gEasyChatWord_Went,
        .alphabeticalOrder = EC_INDEX(EC_WORD_CHOOSE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SAID)] =
    {
        .text = gEasyChatWord_Said,
        .alphabeticalOrder = EC_INDEX(EC_WORD_COLLECT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WAKE_UP)] =
    {
        .text = gEasyChatWord_WakeUp,
        .alphabeticalOrder = EC_INDEX(EC_WORD_COME),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WAKES_UP)] =
    {
        .text = gEasyChatWord_WakesUp,
        .alphabeticalOrder = EC_INDEX(EC_WORD_CONCEDE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ANGERS)] =
    {
        .text = gEasyChatWord_Angers,
        .alphabeticalOrder = EC_INDEX(EC_WORD_COULDN_T),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TEACH)] =
    {
        .text = gEasyChatWord_Teach,
        .alphabeticalOrder = EC_INDEX(EC_WORD_DISAPPEAR),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TEACHES)] =
    {
        .text = gEasyChatWord_Teaches,
        .alphabeticalOrder = EC_INDEX(EC_WORD_DRINKS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PLEASE)] =
    {
        .text = gEasyChatWord_Please,
        .alphabeticalOrder = EC_INDEX(EC_WORD_EATS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LEARN)] =
    {
        .text = gEasyChatWord_Learn,
        .alphabeticalOrder = EC_INDEX(EC_WORD_FAINT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CHANGE)] =
    {
        .text = gEasyChatWord_Change,
        .alphabeticalOrder = EC_INDEX(EC_WORD_FAINTED),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_STORY)] =
    {
        .text = gEasyChatWord_Story,
        .alphabeticalOrder = EC_INDEX(EC_WORD_FORGET),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TRUST)] =
    {
        .text = gEasyChatWord_Trust,
        .alphabeticalOrder = EC_INDEX(EC_WORD_FORGETS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LAVISH)] =
    {
        .text = gEasyChatWord_Lavish,
        .alphabeticalOrder = EC_INDEX(EC_WORD_GIVE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LISTENS)] =
    {
        .text = gEasyChatWord_Listens,
        .alphabeticalOrder = EC_INDEX(EC_WORD_GIVES),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_HEARING)] =
    {
        .text = gEasyChatWord_Hearing,
        .alphabeticalOrder = EC_INDEX(EC_WORD_HEARING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TRAINS)] =
    {
        .text = gEasyChatWord_Trains,
        .alphabeticalOrder = EC_INDEX(EC_WORD_IGNORANT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CHOOSE)] =
    {
        .text = gEasyChatWord_Choose,
        .alphabeticalOrder = EC_INDEX(EC_WORD_KNOW),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_COME)] =
    {
        .text = gEasyChatWord_Come,
        .alphabeticalOrder = EC_INDEX(EC_WORD_KNOWS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CAME)] =
    {
        .text = gEasyChatWord_Came,
        .alphabeticalOrder = EC_INDEX(EC_WORD_LAVISH),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SEARCH)] =
    {
        .text = gEasyChatWord_Search,
        .alphabeticalOrder = EC_INDEX(EC_WORD_LEARN),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_MAKE)] =
    {
        .text = gEasyChatWord_Make,
        .alphabeticalOrder = EC_INDEX(EC_WORD_LISTENS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CAUSE)] =
    {
        .text = gEasyChatWord_Cause,
        .alphabeticalOrder = EC_INDEX(EC_WORD_LOOKS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_KNOW)] =
    {
        .text = gEasyChatWord_Know,
        .alphabeticalOrder = EC_INDEX(EC_WORD_MAKE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_KNOWS)] =
    {
        .text = gEasyChatWord_Knows,
        .alphabeticalOrder = EC_INDEX(EC_WORD_MEETS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_REFUSE)] =
    {
        .text = gEasyChatWord_Refuse,
        .alphabeticalOrder = EC_INDEX(EC_WORD_OVERDO),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_STORES)] =
    {
        .text = gEasyChatWord_Stores,
        .alphabeticalOrder = EC_INDEX(EC_WORD_OWN),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BRAG)] =
    {
        .text = gEasyChatWord_Brag,
        .alphabeticalOrder = EC_INDEX(EC_WORD_PLAYED),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_IGNORANT)] =
    {
        .text = gEasyChatWord_Ignorant,
        .alphabeticalOrder = EC_INDEX(EC_WORD_PLAYS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THINKS)] =
    {
        .text = gEasyChatWord_Thinks,
        .alphabeticalOrder = EC_INDEX(EC_WORD_PLEASE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_BELIEVE)] =
    {
        .text = gEasyChatWord_Believe,
        .alphabeticalOrder = EC_INDEX(EC_WORD_PRAISE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SLIDE)] =
    {
        .text = gEasyChatWord_Slide,
        .alphabeticalOrder = EC_INDEX(EC_WORD_PRETEND),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_EATS)] =
    {
        .text = gEasyChatWord_Eats,
        .alphabeticalOrder = EC_INDEX(EC_WORD_REFUSE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_USE)] =
    {
        .text = gEasyChatWord_Use,
        .alphabeticalOrder = EC_INDEX(EC_WORD_RELEASE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_USES)] =
    {
        .text = gEasyChatWord_Uses,
        .alphabeticalOrder = EC_INDEX(EC_WORD_RUN),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_USING)] =
    {
        .text = gEasyChatWord_Using,
        .alphabeticalOrder = EC_INDEX(EC_WORD_RUNS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_COULDN_T)] =
    {
        .text = gEasyChatWord_Couldnt,
        .alphabeticalOrder = EC_INDEX(EC_WORD_SAID),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_CAPABLE)] =
    {
        .text = gEasyChatWord_Capable,
        .alphabeticalOrder = EC_INDEX(EC_WORD_SAYS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DISAPPEAR)] =
    {
        .text = gEasyChatWord_Disappear,
        .alphabeticalOrder = EC_INDEX(EC_WORD_SEARCH),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_APPEAR)] =
    {
        .text = gEasyChatWord_Appear,
        .alphabeticalOrder = EC_INDEX(EC_WORD_SEEK),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_THROW)] =
    {
        .text = gEasyChatWord_Throw,
        .alphabeticalOrder = EC_INDEX(EC_WORD_SEES),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WORRY)] =
    {
        .text = gEasyChatWord_Worry,
        .alphabeticalOrder = EC_INDEX(EC_WORD_SHOW),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SLEPT)] =
    {
        .text = gEasyChatWord_Slept,
        .alphabeticalOrder = EC_INDEX(EC_WORD_SINK),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SLEEP)] =
    {
        .text = gEasyChatWord_Sleep,
        .alphabeticalOrder = EC_INDEX(EC_WORD_SLEEP),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RELEASE)] =
    {
        .text = gEasyChatWord_Release,
        .alphabeticalOrder = EC_INDEX(EC_WORD_SLEPT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_DRINKS)] =
    {
        .text = gEasyChatWord_Drinks,
        .alphabeticalOrder = EC_INDEX(EC_WORD_SLIDE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RUNS)] =
    {
        .text = gEasyChatWord_Runs,
        .alphabeticalOrder = EC_INDEX(EC_WORD_SMACK),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_RUN)] =
    {
        .text = gEasyChatWord_Run,
        .alphabeticalOrder = EC_INDEX(EC_WORD_STORES),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WORKS)] =
    {
        .text = gEasyChatWord_Works,
        .alphabeticalOrder = EC_INDEX(EC_WORD_STORY),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_WORKING)] =
    {
        .text = gEasyChatWord_Working,
        .alphabeticalOrder = EC_INDEX(EC_WORD_TAKE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TALKING)] =
    {
        .text = gEasyChatWord_Talking,
        .alphabeticalOrder = EC_INDEX(EC_WORD_TALK),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TALK)] =
    {
        .text = gEasyChatWord_Talk,
        .alphabeticalOrder = EC_INDEX(EC_WORD_TALKING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SINK)] =
    {
        .text = gEasyChatWord_Sink,
        .alphabeticalOrder = EC_INDEX(EC_WORD_TEACH),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SMACK)] =
    {
        .text = gEasyChatWord_Smack,
        .alphabeticalOrder = EC_INDEX(EC_WORD_TEACHES),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PRETEND)] =
    {
        .text = gEasyChatWord_Pretend,
        .alphabeticalOrder = EC_INDEX(EC_WORD_THINKS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_PRAISE)] =
    {
        .text = gEasyChatWord_Praise,
        .alphabeticalOrder = EC_INDEX(EC_WORD_THROW),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_OVERDO)] =
    {
        .text = gEasyChatWord_Overdo,
        .alphabeticalOrder = EC_INDEX(EC_WORD_TRAINS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SHOW)] =
    {
        .text = gEasyChatWord_Show,
        .alphabeticalOrder = EC_INDEX(EC_WORD_TRUST),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_LOOKS)] =
    {
        .text = gEasyChatWord_Looks,
        .alphabeticalOrder = EC_INDEX(EC_WORD_USE),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SEES)] =
    {
        .text = gEasyChatWord_Sees,
        .alphabeticalOrder = EC_INDEX(EC_WORD_USES),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_SEEK)] =
    {
        .text = gEasyChatWord_Seek,
        .alphabeticalOrder = EC_INDEX(EC_WORD_USING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_OWN)] =
    {
        .text = gEasyChatWord_Own,
        .alphabeticalOrder = EC_INDEX(EC_WORD_WAKE_UP),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_TAKE)] =
    {
        .text = gEasyChatWord_Take,
        .alphabeticalOrder = EC_INDEX(EC_WORD_WAKES_UP),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_ALLOW)] =
    {
        .text = gEasyChatWord_Allow,
        .alphabeticalOrder = EC_INDEX(EC_WORD_WALKING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FORGET)] =
    {
        .text = gEasyChatWord_Forget,
        .alphabeticalOrder = EC_INDEX(EC_WORD_WALKS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FORGETS)] =
    {
        .text = gEasyChatWord_Forgets,
        .alphabeticalOrder = EC_INDEX(EC_WORD_WENT),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_APPEARS)] =
    {
        .text = gEasyChatWord_Appears,
        .alphabeticalOrder = EC_INDEX(EC_WORD_WORKING),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FAINT)] =
    {
        .text = gEasyChatWord_Faint,
        .alphabeticalOrder = EC_INDEX(EC_WORD_WORKS),
        .enabled = TRUE,
    },
    [EC_INDEX(EC_WORD_FAINTED)] =
    {
        .text = gEasyChatWord_Fainted,
        .alphabeticalOrder = EC_INDEX(EC_WORD_WORRY),
        .enabled = TRUE,
    },
};
