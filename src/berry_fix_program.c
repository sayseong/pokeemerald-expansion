#include "global.h"
#include "gpu_regs.h"
#include "multiboot.h"
#include "malloc.h"
#include "bg.h"
#include "graphics.h"
#include "main.h"
#include "sprite.h"
#include "task.h"
#include "scanline_effect.h"
#include "window.h"
#include "text.h"
#include "menu.h"
#include "m4a.h"
#include "constants/rgb.h"

enum {
    WIN_TITLE,          // "Berry Program Update" header on the first screen
    WIN_MSG_BODY,
    WIN_GAME_NAMES,     // The labels under the GBA graphics on the link screen
    WIN_TURN_OFF_TITLE, // "Ruby/Sapphire" at the top of the "turn off the power" screen
};

struct {
    u8 state;
    u8 curScene;
    u16 timer;
    struct MultiBootParam mb;
} static *sBerryFix;

static void BerryFix_Main(void);
static void BerryFix_GpuSet(void);
static int BerryFix_TrySetScene(int);
static void BerryFix_SetScene(int);
static void BerryFix_HideScene(void);

static const u8 sText_BerryProgramUpdate[] = _("树果问题修复补丁程序");
static const u8 sText_RubySapphire[] = _("红宝石·蓝宝石");
static const u8 sText_Emerald[] = _("绿宝石");
static const u8 sText_BerryProgramWillBeUpdatedPressA[] = _("{COLOR DARK_GRAY}{SHADOW LIGHT_GRAY}{CLEAR_TO 24}针对精灵宝可梦：红宝石·蓝宝石\n"
                                                            "{CLEAR_TO 12}树果问题进行修复的补丁程序已就绪\n"
                                                            "\n{COLOR RED}{SHADOW LIGHT_RED}{CLEAR_TO 8}请按下A键继续。");
static const u8 sText_EnsureGBAConnectionMatches[] = _("{COLOR DARK_GRAY}{SHADOW LIGHT_GRAY}请确认2台GBA游戏机\n"
                                                       "是否已经按照上图所示的方式连接。\n"
                                                       "{COLOR RED}{SHADOW LIGHT_RED}确认，没有问题{CLEAR_TO 12} ▶按下A键\n"
                                                       "不是{CLEAR_TO 12} ▶关机重启再试一次");
static const u8 sText_TurnOffPowerHoldingStartSelect[] = _("{COLOR DARK_GRAY}{SHADOW LIGHT_GRAY}请一边按住START键和SELECT键，一边打开\n"
                                                           "插有宝可梦红蓝宝石卡带的GBA电源。\n"
                                                           "完成以上步骤后，请确认该GBA上\n"
                                                           "是否已经出现上面的开机画面。");
static const u8 sText_TransmittingPleaseWait[] = _("正在连接\n"
                                                   "请稍等……\n"
                                                   "\n{COLOR RED}{SHADOW LIGHT_RED}{CLEAR_TO 24}请勿拔插连接数据线！");
static const u8 sText_PleaseFollowInstructionsOnScreen[] = _("请根据红宝石·蓝宝石\n"
                                                             "游戏画面操作。");
static const u8 sText_TransmissionFailureTryAgain[] = _("\n{CLEAR_TO 12}连接失败！\n"
                                                        "\n{COLOR RED}{SHADOW LIGHT_RED}{CLEAR_TO 12}请关机并重新再试一次。");

static const struct BgTemplate sBerryFixBgTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    }
};

static const struct WindowTemplate sBerryFixWindowTemplates[] = {
    [WIN_TITLE] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 4,
        .width = 26,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 1
    },
    [WIN_MSG_BODY] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 11,
        .width = 28,
        .height = 8,
        .paletteNum = 15,
        .baseBlock = 53
    },
    [WIN_GAME_NAMES] = {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 8,
        .width = 30,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 277
    },
    [WIN_TURN_OFF_TITLE] = {
        .bg = 0,
        .tilemapLeft = 8,
        .tilemapTop = 0,
        .width = 14,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 337
    },
    DUMMY_WIN_TEMPLATE
};

static const u16 ALIGNED(4) sText_Pal[] = INCBIN_U16("graphics/berry_fix/text.gbapal");
static const u8 sBerryProgramTextColors[] = {TEXT_DYNAMIC_COLOR_1, TEXT_DYNAMIC_COLOR_2, TEXT_DYNAMIC_COLOR_3};
static const u8 sGameTitleTextColors[] = { TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_1, TEXT_DYNAMIC_COLOR_4};

enum {
    SCENE_ENSURE_CONNECT,
    SCENE_TURN_OFF_POWER,
    SCENE_TRANSMITTING,
    SCENE_FOLLOW_INSTRUCT,
    SCENE_TRANSMIT_FAILED,
    SCENE_BEGIN,
    SCENE_NONE
};

static const u8 *const sBerryProgramTexts[] = {
    [SCENE_ENSURE_CONNECT]  = sText_EnsureGBAConnectionMatches,
    [SCENE_TURN_OFF_POWER]  = sText_TurnOffPowerHoldingStartSelect,
    [SCENE_TRANSMITTING]    = sText_TransmittingPleaseWait,
    [SCENE_FOLLOW_INSTRUCT] = sText_PleaseFollowInstructionsOnScreen,
    [SCENE_TRANSMIT_FAILED] = sText_TransmissionFailureTryAgain,
    [SCENE_BEGIN]           = sText_BerryProgramWillBeUpdatedPressA
};

static const struct {
    const u32 *gfx;
    const u32 *tilemap;
    const u16 *palette;
} sBerryFixGraphics[] = {
    [SCENE_ENSURE_CONNECT] = {
        gBerryFixGbaConnect_Gfx,
        gBerryFixGbaConnect_Tilemap,
        gBerryFixGbaConnect_Pal
    },
    [SCENE_TURN_OFF_POWER] = {
        gBerryFixGameboyLogo_Gfx,
        gBerryFixGameboyLogo_Tilemap,
        gBerryFixGameboyLogo_Pal
    },
    [SCENE_TRANSMITTING] = {
        gBerryFixGbaTransfer_Gfx,
        gBerryFixGbaTransfer_Tilemap,
        gBerryFixGbaTransfer_Pal
    },
    [SCENE_FOLLOW_INSTRUCT] = {
        gBerryFixGbaTransferHighlight_Gfx,
        gBerryFixGbaTransferHighlight_Tilemap,
        gBerryFixGbaTransferHighlight_Pal
    },
    [SCENE_TRANSMIT_FAILED] = {
        gBerryFixGbaTransferError_Gfx,
        gBerryFixGbaTransferError_Tilemap,
        gBerryFixGbaTransferError_Pal
    },
    [SCENE_BEGIN] = {
        gBerryFixWindow_Gfx,
        gBerryFixWindow_Tilemap,
        gBerryFixWindow_Pal
    },
};

extern const u8 gMultiBootProgram_BerryGlitchFix_Start[0x3BF4];
extern const u8 gMultiBootProgram_BerryGlitchFix_End[];

enum {
    MAINSTATE_INIT,
    MAINSTATE_BEGIN,
    MAINSTATE_CONNECT,
    MAINSTATE_INIT_MULTIBOOT,
    MAINSTATE_MULTIBOOT,
    MAINSTATE_TRANSMIT,
    MAINSTATE_EXIT,
    MAINSTATE_FAILED,
};

void CB2_InitBerryFixProgram(void)
{
    DisableInterrupts(0xFFFF); // all
    EnableInterrupts(INTR_FLAG_VBLANK);
    m4aSoundVSyncOff();
    SetVBlankCallback(NULL);
    ResetSpriteData();
    ResetTasks();
    ScanlineEffect_Stop();
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    sBerryFix = AllocZeroed(sizeof(*sBerryFix));
    sBerryFix->state = MAINSTATE_INIT;
    sBerryFix->curScene = SCENE_NONE;
    SetMainCallback2(BerryFix_Main);
}

#define TryScene(sceneNum) BerryFix_TrySetScene(sceneNum) == (sceneNum)

static void BerryFix_Main(void)
{
    switch (sBerryFix->state)
    {
        case MAINSTATE_INIT:
            BerryFix_GpuSet();
            sBerryFix->state = MAINSTATE_BEGIN;
            break;
        case MAINSTATE_BEGIN:
            if (TryScene(SCENE_BEGIN) && JOY_NEW(A_BUTTON))
                sBerryFix->state = MAINSTATE_CONNECT;
            break;
        case MAINSTATE_CONNECT:
            if (TryScene(SCENE_ENSURE_CONNECT) && JOY_NEW(A_BUTTON))
                sBerryFix->state = MAINSTATE_INIT_MULTIBOOT;
            break;
        case MAINSTATE_INIT_MULTIBOOT:
            if (TryScene(SCENE_TURN_OFF_POWER))
            {
                sBerryFix->mb.masterp = gMultiBootProgram_BerryGlitchFix_Start;
                sBerryFix->mb.server_type = 0;
                MultiBootInit(&sBerryFix->mb);
                sBerryFix->timer = 0;
                sBerryFix->state = MAINSTATE_MULTIBOOT;
            }
            break;
        case MAINSTATE_MULTIBOOT:
            MultiBootMain(&sBerryFix->mb);
            if (sBerryFix->mb.probe_count != 0 || (!(sBerryFix->mb.response_bit & 2) || !(sBerryFix->mb.client_bit & 2)))
            {
                sBerryFix->timer = 0;
            }
            else if (++sBerryFix->timer > 180)
            {
                MultiBootStartMaster(&sBerryFix->mb,
                                     gMultiBootProgram_BerryGlitchFix_Start + ROM_HEADER_SIZE,
                                     (u32)(gMultiBootProgram_BerryGlitchFix_End - (gMultiBootProgram_BerryGlitchFix_Start + ROM_HEADER_SIZE)),
                                     4,
                                     1);
                sBerryFix->state = MAINSTATE_TRANSMIT;
            }
            break;
        case MAINSTATE_TRANSMIT:
            if (TryScene(SCENE_TRANSMITTING))
            {
                MultiBootMain(&sBerryFix->mb);

                if (MultiBootCheckComplete(&sBerryFix->mb))
                    sBerryFix->state = MAINSTATE_EXIT;
                else if (!(sBerryFix->mb.client_bit & 2))
                    sBerryFix->state = MAINSTATE_FAILED;
            }
            break;
        case MAINSTATE_EXIT:
            if (TryScene(SCENE_FOLLOW_INSTRUCT) && JOY_NEW(A_BUTTON))
                DoSoftReset();
            break;
        case MAINSTATE_FAILED:
            if (TryScene(SCENE_TRANSMIT_FAILED) && JOY_NEW(A_BUTTON))
                sBerryFix->state = MAINSTATE_BEGIN;
            break;
    }
}

static void BerryFix_GpuSet(void)
{
    s32 width, left;

    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);

    DmaFill32(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill32(3, 0, PLTT, PLTT_SIZE);
    ResetBgsAndClearDma3BusyFlags(0);

    InitBgsFromTemplates(0, sBerryFixBgTemplates, ARRAY_COUNT(sBerryFixBgTemplates));
    ChangeBgX(0, 0, BG_COORD_SET);
    ChangeBgY(0, 0, BG_COORD_SET);
    ChangeBgX(1, 0, BG_COORD_SET);
    ChangeBgY(1, 0, BG_COORD_SET);
    InitWindows(sBerryFixWindowTemplates);
    DeactivateAllTextPrinters();

    DmaCopy32(3, sText_Pal, BG_PLTT + PLTT_OFFSET_4BPP(15), sizeof(sText_Pal));
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP);
    FillWindowPixelBuffer(WIN_GAME_NAMES, PIXEL_FILL(0));
    FillWindowPixelBuffer(WIN_TURN_OFF_TITLE, PIXEL_FILL(0));
    FillWindowPixelBuffer(WIN_TITLE, PIXEL_FILL(10));

    width = GetStringWidth(FONT_SMALL, sText_Emerald, 0);
    left = (120 - width) / 2;
    AddTextPrinterParameterized3(WIN_GAME_NAMES, FONT_SMALL, left, 3, sGameTitleTextColors, TEXT_SKIP_DRAW, sText_Emerald);

    width = GetStringWidth(FONT_SMALL, sText_RubySapphire, 0);
    left = (120 - width) / 2 + 120;
    AddTextPrinterParameterized3(WIN_GAME_NAMES, FONT_SMALL, left, 3, sGameTitleTextColors, TEXT_SKIP_DRAW, sText_RubySapphire);

    width = GetStringWidth(FONT_SMALL, sText_RubySapphire, 0);
    left = (112 - width) / 2;
    AddTextPrinterParameterized3(WIN_TURN_OFF_TITLE, FONT_SMALL, left, 0, sGameTitleTextColors, TEXT_SKIP_DRAW, sText_RubySapphire);

    width = GetStringWidth(FONT_NORMAL, sText_BerryProgramUpdate, 0);
    left = (208 - width) / 2;
    AddTextPrinterParameterized3(WIN_TITLE, FONT_NORMAL, left, 2, sBerryProgramTextColors, TEXT_SKIP_DRAW, sText_BerryProgramUpdate);

    CopyWindowToVram(WIN_GAME_NAMES, COPYWIN_GFX);
    CopyWindowToVram(WIN_TURN_OFF_TITLE, COPYWIN_GFX);
    CopyWindowToVram(WIN_TITLE, COPYWIN_GFX);
}

static int BerryFix_TrySetScene(int scene)
{
    if (sBerryFix->curScene == scene)
        return scene;

    if (sBerryFix->curScene == SCENE_NONE)
    {
        BerryFix_SetScene(scene);
        sBerryFix->curScene = scene;
    }
    else
    {
        BerryFix_HideScene();
        sBerryFix->curScene = SCENE_NONE;
    }
    return sBerryFix->curScene;
}

static void BerryFix_SetScene(int scene)
{
    FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 32, 32);
    FillWindowPixelBuffer(WIN_MSG_BODY, PIXEL_FILL(10));
    AddTextPrinterParameterized3(WIN_MSG_BODY, FONT_NORMAL, 0, 0, sBerryProgramTextColors, TEXT_SKIP_DRAW, sBerryProgramTexts[scene]);
    PutWindowTilemap(WIN_MSG_BODY);
    CopyWindowToVram(WIN_MSG_BODY, COPYWIN_GFX);
    switch (scene)
    {
    case SCENE_ENSURE_CONNECT:
    case SCENE_TRANSMITTING:
    case SCENE_FOLLOW_INSTRUCT:
    case SCENE_TRANSMIT_FAILED:
        PutWindowTilemap(WIN_GAME_NAMES);
        break;
    case SCENE_TURN_OFF_POWER:
        PutWindowTilemap(WIN_TURN_OFF_TITLE);
        break;
    case SCENE_BEGIN:
        PutWindowTilemap(WIN_TITLE);
        break;
    }
    CopyBgTilemapBufferToVram(0);
    LZ77UnCompVram(sBerryFixGraphics[scene].gfx, (void *)BG_CHAR_ADDR(1));
    LZ77UnCompVram(sBerryFixGraphics[scene].tilemap, (void *)BG_SCREEN_ADDR(31));
    // These palettes range in size from 32-48 colors, so the below is interpreting whatever
    // follows the palette (by default, the corresponding tiles) as the remaining 80-96.
    CpuCopy32(sBerryFixGraphics[scene].palette, (void *)BG_PLTT, PLTT_SIZEOF(128));
    ShowBg(0);
    ShowBg(1);
}

static void BerryFix_HideScene(void)
{
    HideBg(0);
    HideBg(1);
}
