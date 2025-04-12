Route111_Text_MakingRoomUseTMToMakeYourOwn:
	.string "那是什么？\n"
	.string "我在干什么？\p"
	.string "我想利用一个技能\n"
	.string "在这里打出我自己的地方。\p"
	.string "我知道！这个技能机器给你好了。\n"
	.string "用它也来造一个你自己的房间如何？$"

Route111_Text_ExplainSecretPower:
	.string "找一棵看上去可以垂下\n"
	.string "藤蔓的大树，\p"
	.string "在树前面使用神秘力量，\n"
	.string "一些藤蔓就会从树上滑落下来，\l"
	.string "这样你就能爬上树去了。\p"
	.string "你可以在上面找到很多\n"
	.string "放置你喜欢的东西的地方，\p"
	.string "它就成为了你自己的秘密空间…\n"
	.string "一个秘密基地！\p"
	.string "你也做一个吧。\p"
	.string "另外，你不一定要把\n"
	.string "秘密基地建在树上，\p"
	.string "试试对着石头墙壁上的\n"
	.string "小洞使用神秘力量吧。\p"
	.string "我也要去找些别的地方了，\n"
	.string "好了，再见！$"

Route111_Text_DontWantThis:
	.string "哦？你不想要？\n"
	.string "如果你改变主意了，就来告诉我，好吗？$"

Route111_Text_DontHaveAnyRoom:
	.string "哦？你没有地方拿它了。\p"
	.string "我会暂时保管它的，以后\n"
	.string "再来拿吧，好吗？$"

Route111_EventScript_SecretPowerMan::
	lock
	faceplayer
	msgbox Route111_Text_MakingRoomUseTMToMakeYourOwn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route111_EventScript_GiveSecretPower
	msgbox Route111_Text_DontWantThis, MSGBOX_DEFAULT
	release
	end

Route111_EventScript_GiveSecretPower::
	giveitem ITEM_TM_SECRET_POWER
	goto_if_eq VAR_RESULT, FALSE, Route111_EventScript_NoRoomForSecretPower
	msgbox Route111_Text_ExplainSecretPower, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_RECEIVED_SECRET_POWER
	clearflag FLAG_HIDE_SLATEPORT_CITY_TM_SALESMAN
	call_if_eq VAR_FACING, DIR_WEST, Route111_EventScript_SecretPowerManExit
	call_if_eq VAR_FACING, DIR_EAST, Route111_EventScript_SecretPowerManExit
	call_if_eq VAR_FACING, DIR_NORTH, Route111_EventScript_SecretPowerManExitNorth
	removeobject VAR_LAST_TALKED
	release
	end

Route111_EventScript_SecretPowerManExit::
	applymovement VAR_LAST_TALKED, Route111_Movement_SecretPowerManExit
	waitmovement 0
	return

Route111_EventScript_SecretPowerManExitNorth::
	applymovement VAR_LAST_TALKED, Route111_Movement_SecretPowerManExitNorth
	waitmovement 0
	return

Route111_EventScript_NoRoomForSecretPower::
	msgbox Route111_Text_DontHaveAnyRoom, MSGBOX_DEFAULT
	release
	end

Route111_Movement_SecretPowerManExit:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	step_end

Route111_Movement_SecretPowerManExitNorth:
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	step_end
