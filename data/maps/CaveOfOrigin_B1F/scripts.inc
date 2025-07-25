CaveOfOrigin_B1F_MapScripts::
	.byte 0

CaveOfOrigin_B1F_EventScript_Wallace::
	lock
	faceplayer
	msgbox CaveOfOrigin_B1F_Text_WallaceStory, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 60
	playse SE_PIN
	applymovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_Delay48
	waitmovement 0
	delay 30
	applymovement LOCALID_CAVE_OF_ORIGIN_WALLACE, Common_Movement_FacePlayer
	waitmovement 0
	message CaveOfOrigin_B1F_Text_WhereIsRayquaza
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_WheresRayquaza::
	multichoice 0, 0, MULTI_WHERES_RAYQUAZA, FALSE
	switch VAR_RESULT
	case 0, CaveOfOrigin_B1F_EventScript_AtCaveOfOrigin
	case 1, CaveOfOrigin_B1F_EventScript_AtMtPyre
	case 2, CaveOfOrigin_B1F_EventScript_AtSkyPillar
	case 3, CaveOfOrigin_B1F_EventScript_DontRemember
	goto CaveOfOrigin_B1F_EventScript_DontRemember
	end

CaveOfOrigin_B1F_EventScript_AtCaveOfOrigin::
	message CaveOfOrigin_B1F_Text_ButWereInCaveOfOrigin
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_AtMtPyre::
	message CaveOfOrigin_B1F_Text_OldLadyDidntMentionThat
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_DontRemember::
	message CaveOfOrigin_B1F_Text_CantYouRememberSomehow
	waitmessage
	goto CaveOfOrigin_B1F_EventScript_WheresRayquaza

CaveOfOrigin_B1F_EventScript_AtSkyPillar::
	msgbox CaveOfOrigin_B1F_Text_WellHeadToSkyPillar, MSGBOX_DEFAULT
	closemessage
	playse SE_EXIT
	fadescreenspeed FADE_TO_BLACK, 4
	setflag FLAG_WALLACE_GOES_TO_SKY_PILLAR
	setvar VAR_SOOTOPOLIS_CITY_STATE, 3
	removeobject LOCALID_CAVE_OF_ORIGIN_WALLACE
	clearflag FLAG_HIDE_SKY_PILLAR_WALLACE
	fadescreen FADE_FROM_BLACK
	release
	end

CaveOfOrigin_B1F_Text_WallaceStory:
    .string "啊，你就是{PLAYER}{KUN}吗？\n你的活跃表现我早有耳闻。\p我的名字是米可利。\p曾经是琉璃市的道馆馆主，\n不过因为某些原因，\p现在我把管理道馆的事情\n托付给我的老师亚当了。\p…… …… ……\n…… …… ……\p在这里肆虐的2只宝可梦——\n固拉多和盖欧卡，\l被称为超古代宝可梦，\p然而，超古代宝可梦\n并不止这2只……\p在世界的某处\n还存在着第3只——\p没错，那就是被称为烈空坐\n的超古代宝可梦。\p传说在远古时期，\n正是烈空坐平息了\l那2只宝可梦的的斗争。\p可就连我也不清楚它\n如今究竟身在何处……$"

CaveOfOrigin_B1F_Text_WhereIsRayquaza:
    .string "米可利：{PLAYER}{KUN}，\n莫非你知晓如今烈空坐的下落？\p可以的话，\n请务必告诉我。$"

CaveOfOrigin_B1F_Text_ButWereInCaveOfOrigin:
    .string "米可利：觉醒神殿？\p那不就是这里吗！\n拜托你再认真点！\p请你帮我再想想烈空坐\n现在究竟可能会在哪里。$"

CaveOfOrigin_B1F_Text_OldLadyDidntMentionThat:
    .string "米可利：送神山？\p但在我之前去见那里的老婆婆时，\n她并未提及此事。\p我不认为她会刻意地\n隐瞒什么……\p{PLAYER}{KUN}，\n能请你再认真想想吗？$"

CaveOfOrigin_B1F_Text_CantYouRememberSomehow:
    .string "米可利：哎？不记得了？\n唔……这就难办了……\p真的什么也回想\n不起来了吗？$"

CaveOfOrigin_B1F_Text_WellHeadToSkyPillar:
    .string "米可利：天空之柱？\p没错！\n一定就是那里！\p{PLAYER}{KUN}，事不宜迟！\n我们这就出发去天空之柱！$"

