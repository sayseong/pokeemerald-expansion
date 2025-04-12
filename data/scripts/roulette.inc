Roulette_EventScript_Table1::
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_GameCorner_EventScript_NoCoinCase
	setvar VAR_0x8004, 0
	getpokenewsactive POKENEWS_GAME_CORNER
	goto_if_eq VAR_RESULT, FALSE, Roulette_EventScript_Play
	addvar VAR_0x8004, ROULETTE_SPECIAL_RATE
	goto Roulette_EventScript_Play
	end

Roulette_EventScript_Table2::
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, MauvilleCity_GameCorner_EventScript_NoCoinCase
	setvar VAR_0x8004, 1
	getpokenewsactive POKENEWS_GAME_CORNER
	goto_if_eq VAR_RESULT, FALSE, Roulette_EventScript_Play
	addvar VAR_0x8004, ROULETTE_SPECIAL_RATE
	goto Roulette_EventScript_Play
	end

Roulette_EventScript_Play::
	special PlayRoulette
	waitstate
	end

Roulette_Text_PlayMinimumWagerIsX::
	.string "这里的最低赌注是{STR_VAR_1}，\n"
	.string "要玩吗？$"

Roulette_Text_NotEnoughCoins::
	.string "你的代币数量不够。$"

Roulette_Text_SpecialRateTable::
	.string "本日此桌赔率上升！$"

Roulette_Text_ControlsInstruction::
	.string "使用方向键下注，\n"
	.string "按A键开始。$"

Roulette_Text_ItsAHit::
	.string "中了！$"

Roulette_Text_Jackpot::
	.string "大奖！$"

Roulette_Text_NothingDoing::
	.string "什么也没中！$"

Roulette_Text_YouveWonXCoins::
	.string "你赢了{STR_VAR_1}枚代币！$"

Roulette_Text_NoCoinsLeft::
	.string "一枚代币也不剩$"

Roulette_Text_KeepPlaying::
	.string "继续玩吗？$"

Roulette_Text_BoardWillBeCleared::
	.string "台面已清理。$"

@ Unused
Roulette_Text_YouDontHaveACoinCase::
	.string "你没有代币盒。$"

Roulette_Text_CoinCaseIsFull::
	.string "你的代币盒满了！\n"
	.string "可以在柜台把代币换成奖品。$"
