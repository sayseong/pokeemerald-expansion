#include "global.h"
#include "naming_screen.h"
#include "malloc.h"
#include "palette.h"
#include "task.h"
#include "sprite.h"
#include "string_util.h"
#include "window.h"
#include "bg.h"
#include "gpu_regs.h"
#include "pokemon.h"
#include "field_specials.h"
#include "field_player_avatar.h"
#include "event_object_movement.h"
#include "event_data.h"
#include "constants/songs.h"
#include "pokemon_storage_system.h"
#include "graphics.h"
#include "sound.h"
#include "trig.h"
#include "field_effect.h"
#include "pokemon_icon.h"
#include "data.h"
#include "strings.h"
#include "menu.h"
#include "text_window.h"
#include "overworld.h"
#include "walda_phrase.h"
#include "main.h"
#include "constants/event_objects.h"
#include "constants/rgb.h"
#define LETTER_WIDTH 16   // 固定字宽（不管字母本身实际宽度）
#define LETTER_PAD   1
#define START_X      0

enum {
    INPUT_NONE,
    INPUT_DPAD_UP,
    INPUT_DPAD_DOWN,
    INPUT_DPAD_LEFT,
    INPUT_DPAD_RIGHT,
    INPUT_A_BUTTON,
    INPUT_B_BUTTON,
    INPUT_LR_BUTTON,
    INPUT_SELECT,
    INPUT_START, //OK->添加/删除常用字 跳转首字母
	INPUT_R_BUTTON, //中文界面向右翻页
	INPUT_L_BUTTON, //中文界面向左翻页
};

#define KBROW_COUNT 4
#define KBCOL_COUNT 12

enum {
    GFXTAG_BACK_BUTTON,
    GFXTAG_OK_BUTTON,
    GFXTAG_PAGE_SWAP_FRAME,
    GFXTAG_PAGE_SWAP_BUTTON,
    GFXTAG_PAGE_SWAP_UPPER,
    GFXTAG_PAGE_SWAP_LOWER,
    GFXTAG_PAGE_SWAP_OTHERS,
    GFXTAG_CURSOR,
    GFXTAG_CURSOR_SQUISHED,
    GFXTAG_CURSOR_FILLED,
    GFXTAG_INPUT_ARROW,
    GFXTAG_UNDERSCORE,
};

enum {
    PALTAG_PC_ICON,
    PALTAG_PAGE_SWAP_UPPER,
    PALTAG_PAGE_SWAP_LOWER,
    PALTAG_PAGE_SWAP_OTHERS,
    PALTAG_PAGE_SWAP,
    PALTAG_CURSOR,
    PALTAG_BACK_BUTTON,
    PALTAG_OK_BUTTON,
};

enum {
    WIN_KB_PAGE_1, // Which of these two windows is in front is cycled as the player swaps
    WIN_KB_PAGE_2, // Initially WIN_KB_PAGE_1 is in front, with WIN_KB_PAGE_2 on deck
    WIN_TEXT_ENTRY,
    WIN_TEXT_ENTRY_BOX,
    WIN_BANNER,
    WIN_CH,
    WIN_PINYIN,
    WIN_COUNT,
};

// The constants for the pages are needlessly complicated because GF didn't keep the indexing order consistent
// This set is used for sNamingScreen->currentPage. It uses the order that the pages are cycled in
//页
enum {
    KBPAGE_LETTERS_CH,
    KBPAGE_LETTERS_LOWER,
    KBPAGE_LETTERS_UPPER,
    KBPAGE_SYMBOLS,
    KBPAGE_COUNT,
};

// This set is used for initializing a page's keyboard text and getting its number of columns
//键盘内容
enum {
    KEYBOARD_LETTERS_LOWER,
    KEYBOARD_LETTERS_UPPER,
    KEYBOARD_SYMBOLS,
    KEYBOARD_CH,
};


enum {
    KEY_ROLE_CHAR,
    KEY_ROLE_PAGE,
    KEY_ROLE_BACKSPACE,
    KEY_ROLE_OK,
};

enum {
    BUTTON_PAGE,
    BUTTON_BACK,
    BUTTON_OK,
    BUTTON_COUNT,
};

// states for Task_NamingScreen
enum {
    STATE_FADE_IN,
    STATE_WAIT_FADE_IN,
    STATE_HANDLE_INPUT,
    STATE_MOVE_TO_OK_BUTTON,
    STATE_START_PAGE_SWAP,
    STATE_WAIT_PAGE_SWAP,
    STATE_PRESSED_OK,
    STATE_WAIT_SENT_TO_PC_MESSAGE,
    STATE_FADE_OUT,
    STATE_EXIT,
};

// sates for Task_HandleInput
enum
{
    INPUT_STATE_DISABLED,
    INPUT_STATE_ENABLED,
    INPUT_STATE_OVERRIDE,
};

struct NamingScreenTemplate
{
    u8 copyExistingString;
    u8 maxChars;
    u8 iconFunction;
    u8 addGenderIcon;
    u8 initialPage;
    u8 unused;
    const u8 *title;
};

struct NamingScreenData 
{
    u8 state;
    u8 windows[WIN_COUNT];
    u16 inputCharBaseXPos;
    u16 bg1vOffset;
    u16 bg2vOffset;
    u16 bg1Priority;
    u16 bg2Priority;
    u8 bgToReveal;
    u8 bgToHide;
    u8 currentPage;
    u8 cursorSpriteId;
    u8 keyRole;
    const struct NamingScreenTemplate *template;
    u8 templateNum;
    u8 chBufferCount;
    u8 curChRowMax;
    u8 *destBuffer;
    u16 monSpecies;
    u16 monGender;
    u32 monPersonality;
    MainCallback returnCallback;
    const u8* curChChars;
    u32 chCharsPos;
    u8 gTextMode[16];
    u8 textBuffer[16];
    u8 chBuffer[16];
    u8 tileBuffer[0x2000];
    u8 tilemapBuffer1[0x800];
    u8 tilemapBuffer2[0x800];
    u8 tilemapBuffer3[0x800];
    u8 rectBuffer[32*16];
    u8 rectBuffer2[64*16];
};

EWRAM_DATA static struct NamingScreenData *sNamingScreen = NULL;
extern u16 gKeyRepeatStartDelay;

// extern text
extern const u8 gText_YourName[];
extern const u8 gText_BoxName[];
extern const u8 gText_PkmnsNickname[];
extern const u8 gText_TellHimTheWords[];


// start of .rodata
static const u8 sPCIconOff_Gfx[] = INCBIN_U8("graphics/naming_screen/pc_icon_off.4bpp");
static const u8 sPCIconOn_Gfx[] = INCBIN_U8("graphics/naming_screen/pc_icon_on.4bpp");
static const u16 sKeyboard_Pal[] = INCBIN_U16("graphics/naming_screen/keyboard.gbapal");


static const ALIGNED(1) u8 sKeyboardTextColors[] =
{
     TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY,
     TEXT_COLOR_TRANSPARENT, 14, 15
};

static const u8 *const sTransferredToPCMessages[] =
{
    gText_PkmnTransferredSomeonesPC,
    gText_PkmnTransferredLanettesPC,
    gText_PkmnTransferredSomeonesPCBoxFull,
    gText_PkmnTransferredLanettesPCBoxFull
};

static const struct BgTemplate sBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .priority = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 29,
        .priority = 1
    },
    {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 28,
        .priority = 2
    },
    {
        .bg = 3,
        .charBaseIndex = 3,
        .mapBaseIndex = 31,
        .priority = 3
    }
};

static const struct WindowTemplate sWindowTemplates[WIN_COUNT + 1] =
{
    [WIN_KB_PAGE_1] = {
        .bg = 1,
        .tilemapLeft = 3,
        .tilemapTop = 10,
        .width = 19,
        .height = 8,
        .paletteNum = 10,
        .baseBlock = 0x030
    },
    [WIN_KB_PAGE_2] = {//主页面
        .bg = 2,
        .tilemapLeft = 3,
        .tilemapTop = 9,
        .width = 24,
        .height = 8,
        .paletteNum = 10,
        .baseBlock = 0x0C8
    },
    [WIN_TEXT_ENTRY] = {
        .bg = 2,
        .tilemapLeft = 12,
        .tilemapTop = 2,
        .width = 17,
        .height = 2,
        .paletteNum = 10,
        .baseBlock = 0x030
    },
    [WIN_TEXT_ENTRY_BOX] = {
        .bg = 2,
        .tilemapLeft = 8,
        .tilemapTop = 4,
        .width = 17,
        .height = 2,
        .paletteNum = 10,
        .baseBlock = 0x052
    },
    [WIN_BANNER] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 17,
        .width = 30,
        .height = 3,
        .paletteNum = 1,
        .baseBlock = 0x074
    },
    [WIN_CH] = {
        .bg = 2,
        .tilemapLeft = 2,
        .tilemapTop = 6,
        .width = 26,
        .height = 2,
        .paletteNum = 10,
        .baseBlock = 394
    },
    [WIN_PINYIN] = {
        .bg = 2,
        .tilemapLeft = 8,
        .tilemapTop = 0,
        .width = 6,
        .height = 2,
        .paletteNum = 10,
        .baseBlock = 452
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 gText_NamingScreenKeyboard_Upper0[] = _(" QWERTYUIOP ");
static const u8 gText_NamingScreenKeyboard_Upper1[] = _(" ASDFGHJKL  ");
static const u8 gText_NamingScreenKeyboard_Upper2[] = _("  ZXCVBNM   ");
static const u8 gText_NamingScreenKeyboard_Lower0[] = _(" qwertyuiop ");
static const u8 gText_NamingScreenKeyboard_Lower1[] = _(" asdfghjkl  ");
static const u8 gText_NamingScreenKeyboard_Lower2[] = _("  zxcvbnm   ");
static const u8 gText_NamingScreenKeyboard_Symbol0[] = _(" 0123456789 ");
static const u8 gText_NamingScreenKeyboard_Symbol1[] = _(" !?♂♀/+-{PKMN} ");
static const u8 gText_NamingScreenKeyboard_Symbol2[] = _("   …“”‘''   ");


static const u8* const gText_NamingScreenKeyboard_Words[][3] =
{
    [KEYBOARD_LETTERS_LOWER] =
    {
        gText_NamingScreenKeyboard_Lower0,gText_NamingScreenKeyboard_Lower1,gText_NamingScreenKeyboard_Lower2
    },
    [KEYBOARD_LETTERS_UPPER] =
    {
        gText_NamingScreenKeyboard_Upper0,gText_NamingScreenKeyboard_Upper1,gText_NamingScreenKeyboard_Upper2,
    },
    [KEYBOARD_SYMBOLS] =
    {
        gText_NamingScreenKeyboard_Symbol0,gText_NamingScreenKeyboard_Symbol1,gText_NamingScreenKeyboard_Symbol2
    },
    [KEYBOARD_CH] =
    {
        gText_NamingScreenKeyboard_Lower0,gText_NamingScreenKeyboard_Lower1,gText_NamingScreenKeyboard_Lower2
    },
};


static const u8 sColumnCounts[KBPAGE_COUNT][KBROW_COUNT - 1] = {
    [KEYBOARD_LETTERS_LOWER] =
    {
        ARRAY_COUNT(gText_NamingScreenKeyboard_Lower0) - 1,
        ARRAY_COUNT(gText_NamingScreenKeyboard_Lower1) - 1,
        ARRAY_COUNT(gText_NamingScreenKeyboard_Lower2) - 1
    },
    [KEYBOARD_LETTERS_UPPER] =
    {
        ARRAY_COUNT(gText_NamingScreenKeyboard_Upper0) - 1,
        ARRAY_COUNT(gText_NamingScreenKeyboard_Upper1) - 1,
        ARRAY_COUNT(gText_NamingScreenKeyboard_Upper2) - 1
    },
    [KEYBOARD_SYMBOLS] =
    {
        ARRAY_COUNT(gText_NamingScreenKeyboard_Symbol0) - 1,
        ARRAY_COUNT(gText_NamingScreenKeyboard_Symbol1) - 1,
        ARRAY_COUNT(gText_NamingScreenKeyboard_Symbol2) - 1
    },
    [KEYBOARD_CH] =
    {
        ARRAY_COUNT(gText_NamingScreenKeyboard_Lower0) - 1,
        ARRAY_COUNT(gText_NamingScreenKeyboard_Lower1) - 1,
        ARRAY_COUNT(gText_NamingScreenKeyboard_Lower2) - 1
    },
};

static const u32 gNamingScreenBg_Gfx[] = INCBIN_U32("graphics/naming_screen/new/background.4bpp.lz");
static const u32 gNamingScreenBg_Tilemap[] = INCBIN_U32("graphics/naming_screen/new/background.bin.lz");
static const u32 gNamingScreenBg_Pal[] = INCBIN_U32("graphics/naming_screen/new/background.gbapal");
//实际大小23*17
static const u32 gNamingScreenBg_Rect[] = INCBIN_U32("graphics/naming_screen/new/rect.4bpp.lz");
static const u32 gNamingScreenBg_Rect2[] = INCBIN_U32("graphics/naming_screen/new/rect2.4bpp.lz");
// forward declarations
static const struct NamingScreenTemplate *const sNamingScreenTemplates[];
static const struct SubspriteTable sSubspriteTable_PCIcon[];
static const struct SpriteTemplate sSpriteTemplate_Cursor;
static const struct SpriteTemplate sSpriteTemplate_InputArrow;
static const struct SpriteTemplate sSpriteTemplate_Underscore;
static const struct SpriteTemplate sSpriteTemplate_PCIcon;
static const struct SpriteSheet sSpriteSheets[];
static const struct SpritePalette sSpritePalettes[];
static void CopyStrClear(const u8* src, u8* dest, u8 pad);
static bool8 CanDPadMoveToTop();
static bool8 IsPageChMode();
static void CB2_LoadNamingScreen(void);
static void NamingScreen_Init(void);
static void NamingScreen_InitBGs(void);
static void CreateNamingScreenTask(void);
static void Task_NamingScreen(u8 taskId);
static bool8 MainState_FadeIn(void);
static bool8 MainState_WaitFadeIn(void);
static bool8 MainState_HandleInput(void);
static bool8 MainState_MoveToOKButton(void);
static bool8 MainState_PressedOKButton(void);
static bool8 MainState_FadeOut(void);
static bool8 MainState_Exit(void);
static void DisplaySentToPCMessage(void);
static bool8 MainState_WaitSentToPCMessage(void);
static bool8 MainState_StartPageSwap(void);
static bool8 MainState_WaitPageSwap(void);
static void Task_HandlePageSwapAnim(u8);
static bool8 IsPageSwapAnimNotInProgress(void);
static void TryStartButtonFlash(u8, bool8, bool8);
static void Task_UpdateButtonFlash(u8);
static u16 GetButtonPalOffset(u8);
static void CreateSprites(void);
static void CreateCursorSprite(void);
static void SetCursorPos(s16, s16);
static void GetCursorPos(s16 *x, s16 *y);
static void MoveCursorToOKButton(void);
static void SetCursorInvisibility(u8);
static void SetCursorFlashing(bool8);
static u8 IsCursorAnimFinished(void);
static u8 GetCurrentPageColumnCountByRow(s16 y);
static void CreateTextEntrySprites(void);
static void CreateInputTargetIcon(void);
static u8 HandleKeyboardEvent(void);
static u8 SwapKeyboardPage(void);
static u8 GetInputEvent(void);
static void SetInputState(u8);
static u8 GetTextEntryPosition(void);
static void DeleteTextCharacter(void);
static bool8 AddTextCharacter(void);
//static void BufferCharacter(u8);
static void SaveInputText(void);
static void LoadGfx(void);
static void CreateHelperTasks(void);
static void LoadPalettes(void);
static void DrawBgTilemap(u8, const void *);
static void NamingScreen_Dummy(u8, u8);
static void DrawTextEntry(void);
static void PrintKeyboardKeys(u8, u8);
static void PrintControls(u8 page);
static void DrawCotrolHightLightByPage(u8 page);
static void CB2_NamingScreen(void);
static void ResetVHBlank(void);
static void SetVBlank(void);
static void VBlankCB_NamingScreen(void);
static void NamingScreen_ShowBgs(void);
static u16 GetCharAtKeyboardPos(s16 x, s16 y);
static void ReDrawChineseTextEntry();
static void ClearChiniesTextEntry();
static const u8* FindChChars();
static void TurnChinesePage(u8 next);
static const u8* GetCurrentPageChChars();


void DoNamingScreen(u8 templateNum, u8 *destBuffer, u16 monSpecies, u16 monGender, u32 monPersonality, MainCallback returnCallback)
{
    sNamingScreen = Alloc(sizeof(struct NamingScreenData));
    if (!sNamingScreen)
    {
        SetMainCallback2(returnCallback);
    }
    else
    {
        sNamingScreen->templateNum = templateNum;
        sNamingScreen->monSpecies = monSpecies;
        sNamingScreen->monGender = monGender;
        sNamingScreen->monPersonality = monPersonality;
        sNamingScreen->destBuffer = destBuffer;
        sNamingScreen->returnCallback = returnCallback;

        if (templateNum == NAMING_SCREEN_PLAYER)
            StartTimer1();

        SetMainCallback2(CB2_LoadNamingScreen);
    }
}

static void CB2_LoadNamingScreen(void)
{
    switch (gMain.state)
    {
    case 0:
        ResetVHBlank();
        NamingScreen_Init();
        gMain.state++;
        break;
    case 1:
        NamingScreen_InitBGs();
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        gMain.state++;
        break;
    case 3:
        ResetSpriteData();
        FreeAllSpritePalettes();
        gMain.state++;
        break;
    case 4:
        ResetTasks();
        gMain.state++;
        break;
    case 5:
        LoadPalettes();
        gMain.state++;
        break;
    case 6:
        LoadGfx();
        gMain.state++;
        break;
    case 7:
        CreateSprites();
        UpdatePaletteFade();
        NamingScreen_ShowBgs();
        gMain.state++;
        break;
    default:
        CreateHelperTasks();
        CreateNamingScreenTask();
        break;
    }
}

static void NamingScreen_Init(void)
{
    sNamingScreen->state = STATE_FADE_IN;
    sNamingScreen->bg1vOffset = 0;
    sNamingScreen->bg2vOffset = 0;
    sNamingScreen->bg1Priority = BGCNT_PRIORITY(1);
    sNamingScreen->bg2Priority = BGCNT_PRIORITY(2);
    sNamingScreen->bgToReveal = 0;
    sNamingScreen->bgToHide = 1;
    sNamingScreen->template = sNamingScreenTemplates[sNamingScreen->templateNum];
    sNamingScreen->currentPage = sNamingScreen->template->initialPage;
    sNamingScreen->inputCharBaseXPos = (240 - sNamingScreen->template->maxChars * 8) / 2 + 16;
    if (sNamingScreen->templateNum == NAMING_SCREEN_WALDA)
        sNamingScreen->inputCharBaseXPos += 20;
    memset(sNamingScreen->textBuffer, EOS, sizeof(sNamingScreen->textBuffer));
    if (sNamingScreen->template->copyExistingString)
        StringCopy(sNamingScreen->textBuffer, sNamingScreen->destBuffer);
    gKeyRepeatStartDelay = 16;
    sNamingScreen->curChRowMax = 0;
    sNamingScreen->chBufferCount = 0;
    sNamingScreen->curChChars = NULL;
    sNamingScreen->keyRole = KEY_ROLE_CHAR;
}

static void SetSpritesVisible(void)
{
    u8 i;
    for (i = 0; i < MAX_SPRITES; i++)
    {
        if (gSprites[i].inUse)
            gSprites[i].invisible = FALSE;
    }
    SetCursorInvisibility(FALSE);
}

static void NamingScreen_InitBGs(void)
{
    u8 i;

    DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
    DmaClear32(3, (void *)OAM, OAM_SIZE);
    DmaClear16(3, (void *)PLTT, PLTT_SIZE);

    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));

    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(3, 0, 0);

    InitStandardTextBoxWindows();
    InitTextBoxGfxAndPrinters();

    for (i = 0; i < WIN_COUNT; i++)
        sNamingScreen->windows[i] = AddWindow(&sWindowTemplates[i]);

    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(12, 8));

    SetBgTilemapBuffer(1, sNamingScreen->tilemapBuffer1);
    SetBgTilemapBuffer(2, sNamingScreen->tilemapBuffer2);
    SetBgTilemapBuffer(3, sNamingScreen->tilemapBuffer3);

    FillBgTilemapBufferRect_Palette0(1, 0, 0, 0, 0x20, 0x20);
    FillBgTilemapBufferRect_Palette0(2, 0, 0, 0, 0x20, 0x20);
    FillBgTilemapBufferRect_Palette0(3, 0, 0, 0, 0x20, 0x20);
}

static void CreateNamingScreenTask(void)
{
    CreateTask(Task_NamingScreen, 2);
    SetMainCallback2(CB2_NamingScreen);
}

static void Task_NamingScreen(u8 taskId)
{
    switch (sNamingScreen->state)
    {
    case STATE_FADE_IN:
        MainState_FadeIn();
        SetSpritesVisible();
        SetVBlank();
        break;
    case STATE_WAIT_FADE_IN:
        MainState_WaitFadeIn();
        break;
    case STATE_HANDLE_INPUT:
        MainState_HandleInput();
        break;
    case STATE_MOVE_TO_OK_BUTTON:
        MainState_MoveToOKButton();
        MainState_HandleInput();
        break;
    case STATE_START_PAGE_SWAP:
        MainState_StartPageSwap();
        break;
    case STATE_WAIT_PAGE_SWAP:
        MainState_WaitPageSwap();
        break;
    case STATE_PRESSED_OK:
        MainState_PressedOKButton();
        break;
    case STATE_WAIT_SENT_TO_PC_MESSAGE:
        MainState_WaitSentToPCMessage();
        break;
    case STATE_FADE_OUT:
        MainState_FadeOut();
        break;
    case STATE_EXIT:
        MainState_Exit();
        break;
    }
}


static const u8 sPageToNextKeyboardId[KBPAGE_COUNT] = 
{
    [KBPAGE_LETTERS_CH]    = KEYBOARD_LETTERS_LOWER,
    [KBPAGE_LETTERS_LOWER] = KEYBOARD_LETTERS_UPPER,
    [KBPAGE_LETTERS_UPPER] = KEYBOARD_SYMBOLS,
    [KBPAGE_SYMBOLS]       = KEYBOARD_CH,
};

static const u8 sPageToKeyboardId[KBPAGE_COUNT] = 
{ 
    [KBPAGE_SYMBOLS]       = KEYBOARD_SYMBOLS, 
    [KBPAGE_LETTERS_UPPER] = KEYBOARD_LETTERS_UPPER, 
    [KBPAGE_LETTERS_LOWER] = KEYBOARD_LETTERS_LOWER,
    [KBPAGE_LETTERS_CH] =   KEYBOARD_CH,
};


static u8 CurrentPageToNextKeyboardId(void)
{
    return sPageToNextKeyboardId[sNamingScreen->currentPage];
}

static u8 CurrentPageToKeyboardId(void)
{
    return sPageToKeyboardId[sNamingScreen->currentPage];
}

static bool8 MainState_FadeIn(void)
{
    DrawBgTilemap(3, gNamingScreenBg_Tilemap);
    sNamingScreen->currentPage = KBPAGE_LETTERS_CH;
//    DrawBgTilemap(2, gNamingScreenKeyboardLower_Tilemap);
//    DrawBgTilemap(1, gNamingScreenKeyboardUpper_Tilemap);
    PrintKeyboardKeys(sNamingScreen->windows[WIN_KB_PAGE_2], CurrentPageToKeyboardId());
//    PrintKeyboardKeys(sNamingScreen->windows[WIN_KB_PAGE_1], KEYBOARD_LETTERS_UPPER);
    NamingScreen_Dummy(2, KEYBOARD_LETTERS_LOWER);
    NamingScreen_Dummy(1, KEYBOARD_LETTERS_UPPER);
    DrawTextEntry();
//    DrawTextEntryBox();
    DrawCotrolHightLightByPage(KBPAGE_LETTERS_CH);
    CopyBgTilemapBufferToVram(1);
    CopyBgTilemapBufferToVram(2);
    CopyBgTilemapBufferToVram(3);
    BlendPalettes(-1, 16, 0);
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
    sNamingScreen->state++;
    return FALSE;
}

static bool8 MainState_WaitFadeIn(void)
{
    if (!gPaletteFade.active)
    {
        SetInputState(INPUT_STATE_ENABLED);
        SetCursorFlashing(TRUE);
        sNamingScreen->state++;
    }
    return FALSE;
}

static bool8 MainState_HandleInput(void)
{
    return HandleKeyboardEvent();
}

static bool8 MainState_MoveToOKButton(void)
{
    if (IsCursorAnimFinished())
    {
        SetInputState(INPUT_STATE_ENABLED);
        MoveCursorToOKButton();
        sNamingScreen->state = STATE_HANDLE_INPUT;
    }
    return FALSE;
}

static bool8 MainState_PressedOKButton(void)
{
    SaveInputText();
    SetInputState(INPUT_STATE_DISABLED);
    SetCursorFlashing(FALSE);
    TryStartButtonFlash(BUTTON_COUNT, FALSE, TRUE);
    sNamingScreen->state = STATE_FADE_OUT;
    return TRUE;
}

static bool8 MainState_FadeOut(void)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
    sNamingScreen->state++;
    return FALSE;
}

static bool8 MainState_Exit(void)
{
    if (!gPaletteFade.active)
    {
        if (sNamingScreen->templateNum == NAMING_SCREEN_PLAYER)
            SeedRngAndSetTrainerId();
        if (sNamingScreen->templateNum == NAMING_SCREEN_CAUGHT_MON
         && CalculatePlayerPartyCount() < PARTY_SIZE)
            SetMainCallback2(BattleMainCB2);
        else
            SetMainCallback2(sNamingScreen->returnCallback);
        DestroyTask(FindTaskIdByFunc(Task_NamingScreen));
        FreeAllWindowBuffers();
        FREE_AND_SET_NULL(sNamingScreen);
    }
    return FALSE;
}

static UNUSED void DisplaySentToPCMessage(void)
{
    u8 stringToDisplay = 0;

    if (!IsDestinationBoxFull())
    {
        StringCopy(gStringVar1, GetBoxNamePtr(VarGet(VAR_PC_BOX_TO_SEND_MON)));
        StringCopy(gStringVar2, sNamingScreen->destBuffer);
    }
    else
    {
        StringCopy(gStringVar1, GetBoxNamePtr(VarGet(VAR_PC_BOX_TO_SEND_MON)));
        StringCopy(gStringVar2, sNamingScreen->destBuffer);
        StringCopy(gStringVar3, GetBoxNamePtr(GetPCBoxToSendMon()));
        stringToDisplay = 2;
    }

    if (FlagGet(FLAG_SYS_PC_LANETTE))
        stringToDisplay++;

    StringExpandPlaceholders(gStringVar4, sTransferredToPCMessages[stringToDisplay]);
    DrawDialogueFrame(0, 0);
    gTextFlags.canABSpeedUpPrint = TRUE;
    AddTextPrinterParameterized2(0, 1, gStringVar4, GetPlayerTextSpeedDelay(), 0, 2, 1, 3);
    CopyWindowToVram(0, 3);
}

static bool8 MainState_WaitSentToPCMessage(void)
{
    RunTextPrinters();
    if (!IsTextPrinterActive(0) && JOY_NEW(A_BUTTON))
        sNamingScreen->state = STATE_FADE_OUT;

    return FALSE;
}

static const u8 gHightLightConfig[][2] =
{
    [KBPAGE_LETTERS_CH] = {6, 32},
    [KBPAGE_LETTERS_LOWER] = {30, 32},
    [KBPAGE_LETTERS_UPPER] = {30, 32},
    [KBPAGE_SYMBOLS] = {54, 32},
};



static void DrawCotrolHightLight(u8 x, u8 width, u8 type, u8 page)
{
    FillWindowPixelBuffer(sNamingScreen->windows[WIN_BANNER], PIXEL_FILL(0));
    BlitBitmapToWindow(sNamingScreen->windows[WIN_BANNER], type == 0?sNamingScreen->rectBuffer:sNamingScreen->rectBuffer2, x, 5, width, 32);
    PrintControls(page);
}

static void DrawCotrolHightLightByPage(u8 page)
{
    DrawCotrolHightLight(gHightLightConfig[page][0], gHightLightConfig[page][1], 0, page);
}

static bool8 MainState_StartPageSwap(void)
{
    SetInputState(INPUT_STATE_DISABLED);
    SetCursorInvisibility(TRUE);
    TryStartButtonFlash(BUTTON_PAGE, FALSE, TRUE);
    PrintKeyboardKeys(sNamingScreen->windows[WIN_KB_PAGE_2], CurrentPageToNextKeyboardId());
    PlaySE(SE_WIN_OPEN);
    sNamingScreen->state = STATE_WAIT_PAGE_SWAP;
    return FALSE;
}

static bool8 MainState_WaitPageSwap(void)
{
    s16 cursorX;
    s16 cursorY;
    u8 column;

    if (IsPageSwapAnimNotInProgress())
    {

        GetCursorPos(&cursorX, &cursorY);
        column = GetCurrentPageColumnCountByRow(cursorY);
        
         if (cursorX >= column)
             cursorX = column - 1;
        if (IsPageChMode() && cursorY == 0)
        {
            cursorY = 1;
            cursorX = 0;
        }
        SetCursorPos(cursorX, cursorY);
        sNamingScreen->state = STATE_HANDLE_INPUT;
        sNamingScreen->currentPage++;
        sNamingScreen->currentPage %= KBPAGE_COUNT;
        SetInputState(INPUT_STATE_ENABLED);
        SetCursorInvisibility(FALSE);
        ClearChiniesTextEntry();
        DrawCotrolHightLightByPage(sNamingScreen->currentPage);
    }
    return FALSE;
}

//--------------------------------------------------
// Page Swap
//--------------------------------------------------

#define tState data[0]
#define tFrameCount data[1]

static bool8 PageSwapAnimState_Init(struct Task *);
static bool8 PageSwapAnimState_1(struct Task *);
static bool8 PageSwapAnimState_2(struct Task *);
static bool8 PageSwapAnimState_Done(struct Task *);

static bool8 (*const sPageSwapAnimStateFuncs[])(struct Task *) =
{
    PageSwapAnimState_Init,
    PageSwapAnimState_1,
    PageSwapAnimState_2,
    PageSwapAnimState_Done,
};


static void Task_HandlePageSwapAnim(u8 taskId)
{
    while (sPageSwapAnimStateFuncs[gTasks[taskId].tState](&gTasks[taskId]) != 0);
}

static bool8 IsPageSwapAnimNotInProgress(void)
{
    if (FindTaskIdByFunc(Task_HandlePageSwapAnim) == 0xFF)
        return TRUE;
    else
        return FALSE;
}

static bool8 PageSwapAnimState_Init(struct Task *task)
{
    sNamingScreen->bg1vOffset = 0;
    sNamingScreen->bg2vOffset = 0;
    task->tState++;
    return 0;
}

static bool8 PageSwapAnimState_1(struct Task *task)
{
    u16 *const vOffsets[] =
    {
        &sNamingScreen->bg2vOffset,
        &sNamingScreen->bg1vOffset
    };

    task->tFrameCount += 4;
    *vOffsets[sNamingScreen->bgToReveal] = Sin(task->tFrameCount, 40);
    *vOffsets[sNamingScreen->bgToHide] = Sin((task->tFrameCount + 128) & 0xFF, 40);
    if (task->tFrameCount >= 64)
    {
        u8 temp = sNamingScreen->bg1Priority;  //Why u8 and not u16?

        sNamingScreen->bg1Priority = sNamingScreen->bg2Priority;
        sNamingScreen->bg2Priority = temp;
        task->tState++;
    }
    return 0;
}

static bool8 PageSwapAnimState_2(struct Task *task)
{
    u16 *const vOffsets[] =
    {
        &sNamingScreen->bg2vOffset,
        &sNamingScreen->bg1vOffset
    };

    task->tFrameCount += 4;
    *vOffsets[sNamingScreen->bgToReveal] = Sin(task->tFrameCount, 40);
    *vOffsets[sNamingScreen->bgToHide] = Sin((task->tFrameCount + 128) & 0xFF, 40);
    if (task->tFrameCount >= 128)
    {
        u8 temp = sNamingScreen->bgToReveal;

        sNamingScreen->bgToReveal = sNamingScreen->bgToHide;
        sNamingScreen->bgToHide = temp;
        task->tState++;
    }
    return 0;
}

static bool8 PageSwapAnimState_Done(struct Task *task)
{
    DestroyTask(FindTaskIdByFunc(Task_HandlePageSwapAnim));
    return 0;
}

#undef tState
#undef tFrameCount

//--------------------------------------------------
//
//--------------------------------------------------

#define tButtonId     data[0]
#define tKeepFlashing data[1]
#define tAllowFlash   data[2]
#define tColor        data[3]
#define tColorIncr    data[4]
#define tColorDelay   data[5]
#define tColorDelta   data[6]

static void CreateButtonFlashTask(void)
{
    u8 taskId;

    taskId = CreateTask(Task_UpdateButtonFlash, 3);
    gTasks[taskId].tButtonId = BUTTON_COUNT;
}

static void TryStartButtonFlash(u8 button, bool8 keepFlashing, bool8 interruptCurFlash)
{

}

static void Task_UpdateButtonFlash(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (task->tButtonId == BUTTON_COUNT || !task->tAllowFlash)
        return;

    MultiplyInvertedPaletteRGBComponents(GetButtonPalOffset(task->tButtonId), task->tColor, task->tColor, task->tColor);

    if (task->tColorDelay && --task->tColorDelay)
        return;

    task->tColorDelay = 2;
    if (task->tColorIncr >= 0)
    {
        if (task->tColor < 14)
        {
            task->tColor += task->tColorIncr;
            task->tColorDelta += task->tColorIncr;
        }
        else
        {
            task->tColor = 16;
            task->tColorDelta++;
        }
    }
    else
    {
        task->tColor += task->tColorIncr;
        task->tColorDelta += task->tColorIncr;
    }

    if (task->tColor == 16 && task->tColorDelta == 22)
    {
        task->tColorIncr = -4;
    }
    else if (task->tColor == 0)
    {
        task->tAllowFlash = task->tKeepFlashing;
        task->tColorIncr = 2;
        task->tColorDelta = 0;
    }
}

static u16 GetButtonPalOffset(u8 button)
{
    const u16 palOffsets[BUTTON_COUNT + 1] =
    {
        [BUTTON_PAGE]  = IndexOfSpritePaletteTag(PALTAG_PAGE_SWAP) * 16 + 0x10E,
        [BUTTON_BACK]  = IndexOfSpritePaletteTag(PALTAG_BACK_BUTTON) * 16 + 0x10E,
        [BUTTON_OK]    = IndexOfSpritePaletteTag(PALTAG_OK_BUTTON) * 16 + 0x10E,
        [BUTTON_COUNT] = IndexOfSpritePaletteTag(PALTAG_OK_BUTTON) * 16 + 0x101,
    };

    return palOffsets[button];
}

#undef tButtonId
#undef tColor

// Sprite data for the the cursor
#define sX          data[0]
#define sY          data[1]
#define sPrevX      data[2]
#define sPrevY      data[3]
#define sInvisible  data[4] & 0x00FF
#define sFlashing   data[4] & 0xFF00
#define sColor      data[5]
#define sColorIncr  data[6]
#define sColorDelay data[7]

static void SpriteCB_Cursor(struct Sprite *sprite)
{
    if (sprite->animEnded)
        StartSpriteAnim(sprite, 0);

    // Hide cursor when on button column
    sprite->invisible = sprite->sInvisible;
    if (sprite->sX == GetCurrentPageColumnCountByRow(sprite->sY))
        sprite->invisible = TRUE;

    if (sprite->invisible 
       || !(sprite->sFlashing)
       /*|| sprite->sX != sprite->sPrevX
       || sprite->sY != sprite->sPrevY*/)
    {
        sprite->sColor = 0;
        sprite->sColorIncr = 2;
        sprite->sColorDelay = 2;
    }

    sprite->sColorDelay--;
    if (sprite->sColorDelay == 0)
    {
        sprite->sColor += sprite->sColorIncr;
        if (sprite->sColor == 16 || sprite->sColor == 0)
            sprite->sColorIncr = -sprite->sColorIncr;
        sprite->sColorDelay = 2;
    }

    if (sprite->sFlashing)
    {
        s8 gb = sprite->sColor;
        s8 r = sprite->sColor >> 1;
        u16 index = IndexOfSpritePaletteTag(PALTAG_CURSOR) * 16 + 0x0101;

        MultiplyInvertedPaletteRGBComponents(index, r, gb, gb);
    }
}

#define sDelay  data[0]
#define sXPosId data[1]

static void SpriteCB_InputArrow(struct Sprite *sprite)
{
    const s16 x[] = {0, -4, -2, -1};

    if (sprite->sDelay == 0 || --sprite->sDelay == 0)
    {
        sprite->sDelay = 8;
        sprite->sXPosId = (sprite->sXPosId + 1) & (ARRAY_COUNT(x) - 1);
    }
    sprite->x2 = x[sprite->sXPosId];
}

#undef sDelay
#undef sXPosId

#define sId     data[0] // set in CreateTextEntrySprites
#define sYPosId data[1]
#define sDelay  data[2]

static void SpriteCB_Underscore(struct Sprite *sprite)
{
    const s16 y[] = {2, 3, 2, 1};
    u8 pos;

    pos = GetTextEntryPosition();
    if (pos != (u8)sprite->sId)
    {
        sprite->y2 = 0;
        sprite->sYPosId = 0;
        sprite->sDelay = 0;
    }
    else
    {
        sprite->y2 = y[sprite->sYPosId];
        sprite->sDelay++;
        if (sprite->sDelay > 8)
        {
            sprite->sYPosId = (sprite->sYPosId + 1) & (ARRAY_COUNT(y) - 1);
            sprite->sDelay = 0;
        }
    }
}

#undef sId
#undef sYPosId
#undef sDelay

static void CreateSprites(void)
{
    CreateCursorSprite();
//    CreatePageSwapButtonSprites();
//    CreateBackOkSprites();
    CreateTextEntrySprites();
    CreateInputTargetIcon();
}

static void CreateCursorSprite(void)
{
    sNamingScreen->cursorSpriteId = CreateSprite(&sSpriteTemplate_Cursor, 38, 88, 1);
    SetCursorInvisibility(TRUE);
    gSprites[sNamingScreen->cursorSpriteId].oam.priority = 1;
    gSprites[sNamingScreen->cursorSpriteId].oam.objMode = ST_OAM_OBJ_BLEND;
    gSprites[sNamingScreen->cursorSpriteId].sColorIncr = 1; // ? immediately overwritten
    gSprites[sNamingScreen->cursorSpriteId].sColorIncr = 2;
    SetCursorPos(0, 1);
}
#define ROW_HEIGHT 24
#define COLOUM_PAD 11
static void SetCursorPos(s16 x, s16 y)
{
    struct Sprite *cursorSprite = &gSprites[sNamingScreen->cursorSpriteId];

    if (x < GetCurrentPageColumnCountByRow(y))
        cursorSprite->x = x * (COLOUM_PAD + 6) + 26;
    else
        cursorSprite->x = 0;

    cursorSprite->y = y * ROW_HEIGHT + 55;
    cursorSprite->sPrevX = cursorSprite->sX;
    cursorSprite->sPrevY = cursorSprite->sY;
    cursorSprite->sX = x;
    cursorSprite->sY = y;
}

static void GetCursorPos(s16 *x, s16 *y)
{
    struct Sprite *cursorSprite = &gSprites[sNamingScreen->cursorSpriteId];

    *x = cursorSprite->sX;
    *y = cursorSprite->sY;
}

static void MoveCursorToOKButton(void)
{
    DrawCotrolHightLight(145, 64, 1, sNamingScreen->currentPage);
    sNamingScreen->keyRole = KEY_ROLE_OK;
}

static void SetCursorInvisibility(bool8 invisible)
{
    gSprites[sNamingScreen->cursorSpriteId].data[4] &= 0xFF00;
    gSprites[sNamingScreen->cursorSpriteId].data[4] |= invisible; // sInvisible
    StartSpriteAnim(&gSprites[sNamingScreen->cursorSpriteId], 0);
}

static void SetCursorFlashing(bool8 flashing)
{
    gSprites[sNamingScreen->cursorSpriteId].data[4] &= 0xFF;
    gSprites[sNamingScreen->cursorSpriteId].data[4] |= flashing << 8; // sFlashing
}

static void SquishCursor(void)
{
    StartSpriteAnim(&gSprites[sNamingScreen->cursorSpriteId], 1);
}

static bool8 IsCursorAnimFinished(void)
{
    return gSprites[sNamingScreen->cursorSpriteId].animEnded;
}

static u8 GetKeyRoleAtCursorPos(void)
{
    return sNamingScreen->keyRole;
}


static u8 GetCurrentPageColumnCountByRow(s16 y)
{
    if (y == 0)
        return sNamingScreen->curChRowMax;
    return sColumnCounts[CurrentPageToKeyboardId()][y - 1];
}

#undef sX
#undef sY
#undef sPrevX
#undef sPrevY
#undef sInvisible
#undef sFlashing
#undef sColor
#undef sColorIncr
#undef sColorDelay

#define sState          data[0]
#define sPage           data[1]
#define sTextSpriteId   data[6]
#define sButtonSpriteId data[7]



#undef sState
#undef sPage
#undef sTextSpriteId
#undef sButtonSpriteId


static void CreateTextEntrySprites(void)
{
    u8 spriteId;
    s16 xPos;
    u8 i;
    const u8 arrowY= 26;
    const u8 textY = 30;
    xPos = sNamingScreen->inputCharBaseXPos - 5;
    spriteId = CreateSprite(&sSpriteTemplate_InputArrow, xPos, arrowY, 0);
    gSprites[spriteId].oam.priority = 3;
    gSprites[spriteId].invisible = TRUE;
    xPos = sNamingScreen->inputCharBaseXPos;
    for (i = 0; i < sNamingScreen->template->maxChars; i++, xPos += 8)
    {
        spriteId = CreateSprite(&sSpriteTemplate_Underscore, xPos + 3, textY, 0);
        gSprites[spriteId].oam.priority = 3;
        gSprites[spriteId].data[0] = i;
        gSprites[spriteId].invisible = TRUE;
    }
}

//--------------------------------------------------
// Icon creation (the thing you're naming or giving input to)
//--------------------------------------------------

static void NamingScreen_NoIcon(void);
static void NamingScreen_CreatePlayerIcon(void);
static void NamingScreen_CreatePCIcon(void);
static void NamingScreen_CreateMonIcon(void);
static void NamingScreen_CreateWaldaDadIcon(void);
static void NamingScreen_CreateCodeIcon(void);

static void (*const sIconFunctions[])(void) =
{
    NamingScreen_NoIcon,
    NamingScreen_CreatePlayerIcon,
    NamingScreen_CreatePCIcon,
    NamingScreen_CreateMonIcon,
    NamingScreen_CreateWaldaDadIcon,
    NamingScreen_CreateCodeIcon,
};

static void CreateInputTargetIcon(void)
{
    sIconFunctions[sNamingScreen->template->iconFunction]();
}

static void NamingScreen_NoIcon(void)
{

}
#define POS_ICON_X 26
#define POS_ICON_Y 22
static void NamingScreen_CreatePlayerIcon(void)
{
    u16 rivalGfxId;
    u8 spriteId;

    rivalGfxId = GetRivalAvatarGraphicsIdByStateIdAndGender(0, sNamingScreen->monSpecies);
    spriteId = CreateObjectGraphicsSprite(rivalGfxId, SpriteCallbackDummy, POS_ICON_X, POS_ICON_Y - 3, 0);
    gSprites[spriteId].oam.priority = 3;
    StartSpriteAnim(&gSprites[spriteId], ANIM_STD_GO_SOUTH);
}

static void NamingScreen_CreatePCIcon(void)
{
    u8 spriteId;

    spriteId = CreateSprite(&sSpriteTemplate_PCIcon, POS_ICON_X, POS_ICON_Y + 1, 0);
    SetSubspriteTables(&gSprites[spriteId], sSubspriteTable_PCIcon);
    gSprites[spriteId].oam.priority = 3;
}

static void NamingScreen_CreateMonIcon(void)
{
    u8 spriteId;

    LoadMonIconPalettes();
    spriteId = CreateMonIcon(sNamingScreen->monSpecies, SpriteCallbackDummy, POS_ICON_X, POS_ICON_Y, 0,
        sNamingScreen->monPersonality);
    gSprites[spriteId].oam.priority = 3;
}

static void NamingScreen_CreateWaldaDadIcon(void)
{
    u8 spriteId;

    spriteId = CreateObjectGraphicsSprite(OBJ_EVENT_GFX_MAN_1, SpriteCallbackDummy, POS_ICON_X, POS_ICON_Y - 3, 0);
    gSprites[spriteId].oam.priority = 3;
    StartSpriteAnim(&gSprites[spriteId], 4);
//    StartSpriteAnim(&gSprites[spriteId], ANIM_STD_GO_SOUTH);
}

static void NamingScreen_CreateCodeIcon(void)
{
    u8 spriteId;
    spriteId = CreateObjectGraphicsSprite(OBJ_EVENT_GFX_MYSTERY_GIFT_MAN, SpriteCallbackDummy, 56, 37, 0);
    gSprites[spriteId].oam.priority = 3;
}

//--------------------------------------------------
// Keyboard handling
//--------------------------------------------------

static bool8 KeyboardKeyHandler_Character(u8);
static bool8 KeyboardKeyHandler_Page(u8);
static bool8 KeyboardKeyHandler_Backspace(u8);
static bool8 KeyboardKeyHandler_OK(u8);

static bool8 (*const sKeyboardKeyHandlers[])(u8) =
{
    [KEY_ROLE_CHAR]      = KeyboardKeyHandler_Character,
    [KEY_ROLE_PAGE]      = KeyboardKeyHandler_Page,
    [KEY_ROLE_BACKSPACE] = KeyboardKeyHandler_Backspace,
    [KEY_ROLE_OK]        = KeyboardKeyHandler_OK,
};


bool8 Array16Contains(const u16 *array, u16 size, u16 ch)
{
    u16 i;
    for (i = 0; i < size; i++) {
        if (array[i] == ch) {
            return TRUE;
        }
    }
    return FALSE;
}

s32 BinarySearch(const u16 array[], u16 size, u16 key)
{
    u32 left = 0, right = size - 1;
    while (left <= right) {
        u32 mid = (right + left) / 2;
        if (array[mid] > key) {
            right = mid - 1;
        } else if (array[mid] < key) {
            left = mid + 1;
        } else {
            return mid;
        }
    }
    return -1;
}


static bool8 HandleKeyboardEvent(void)
{
	u8 event = GetInputEvent();
    u8 keyRole = GetKeyRoleAtCursorPos();
	bool8 ischinesepage = IsPageChMode();

    if (INPUT_B_BUTTON == event)
	{
		DeleteTextCharacter();
        return FALSE;
	}
	else if (INPUT_SELECT == event)
		return SwapKeyboardPage();
	else if (INPUT_R_BUTTON == event && ischinesepage) //翻到下一页
	{
		TurnChinesePage(TRUE);
		return FALSE;
	}
	else if (INPUT_L_BUTTON == event && ischinesepage) //翻到上一页
	{
        TurnChinesePage(FALSE);
		return FALSE;
	}
	else if (INPUT_START == event)
	{
        MoveCursorToOKButton();
		return FALSE;
	}
	else
		return sKeyboardKeyHandlers[keyRole](event);
}

//static void DeleteCommonWord(void)
//{
//	u8 index;
//	s16 x, y;
//	register u16 i;
//
//	GetCursorPos(&x, &y);
//	index = x + y * 12;
//
//	if (gSaveBlock1Ptr->gCommonWord[index])
//	{
//		for (i = index; i < 47; i++)
//			gSaveBlock1Ptr->gCommonWord[i] = gSaveBlock1Ptr->gCommonWord[i + 1];
//		gSaveBlock1Ptr->gCommonWord[i] = 0;
//	}
//
//	ReDrawKeyboardPage();
//}


static bool8 KeyboardKeyHandler_Character(u8 input)
{
    TryStartButtonFlash(BUTTON_COUNT, FALSE, FALSE);
    if (input == INPUT_A_BUTTON)
    {
        bool8 textFull = AddTextCharacter();

        SquishCursor();
        if (textFull)
        {
            SetInputState(INPUT_STATE_OVERRIDE);
            sNamingScreen->state = STATE_MOVE_TO_OK_BUTTON;
        }
    }
    return FALSE;
}

static bool8 KeyboardKeyHandler_Page(u8 input)
{
    TryStartButtonFlash(BUTTON_PAGE, TRUE, FALSE);
    if (input == INPUT_A_BUTTON)
        return SwapKeyboardPage();
    else
        return FALSE;
}

static bool8 KeyboardKeyHandler_Backspace(u8 input)
{
    TryStartButtonFlash(BUTTON_BACK, TRUE, FALSE);
    if (input == INPUT_A_BUTTON)
        DeleteTextCharacter();
    return FALSE;
}

static bool8 KeyboardKeyHandler_OK(u8 input)
{
    TryStartButtonFlash(BUTTON_OK, TRUE, FALSE);
    if (input == INPUT_A_BUTTON)
    {
        PlaySE(SE_SELECT);
        sNamingScreen->state = STATE_PRESSED_OK;
        return TRUE;
    }
    else
    {
        if (input != INPUT_NONE)
        {
            DrawCotrolHightLightByPage(sNamingScreen->currentPage);
            sNamingScreen->keyRole = KEY_ROLE_CHAR;
        }
        return FALSE;
    }
}

static bool8 SwapKeyboardPage(void)
{
    sNamingScreen->state = STATE_START_PAGE_SWAP;
    return TRUE;
}

//--------------------------------------------------
// Input handling
//--------------------------------------------------

#define tState data[0]
#define tKeyboardEvent data[1]
#define tButtonId data[2]


static void Input_Disabled(struct Task *);
static void Input_Enabled(struct Task *);
static void Input_Override(struct Task *);

static void (*const sInputFuncs[])(struct Task *) =
{
    [INPUT_STATE_DISABLED] = Input_Disabled,
    [INPUT_STATE_ENABLED]  = Input_Enabled,
    [INPUT_STATE_OVERRIDE] = Input_Override,
};

static void Task_HandleInput(u8);
static void HandleDpadMovement(struct Task *);

static void CreateInputHandlerTask(void)
{
    CreateTask(Task_HandleInput, 1);
}

static u8 GetInputEvent(void)
{
    u8 taskId = FindTaskIdByFunc(Task_HandleInput);

    return gTasks[taskId].tKeyboardEvent;
}

static void SetInputState(u8 state)
{
    u8 taskId = FindTaskIdByFunc(Task_HandleInput);

    gTasks[taskId].tState = state;
}

static void Task_HandleInput(u8 taskId)
{
    sInputFuncs[gTasks[taskId].tState](&gTasks[taskId]);
}

static void Input_Disabled(struct Task *task)
{
    task->tKeyboardEvent = INPUT_NONE;
}

static void Input_Enabled(struct Task *task)
{
    task->tKeyboardEvent = INPUT_NONE;

    if (JOY_NEW(A_BUTTON))
        task->tKeyboardEvent = INPUT_A_BUTTON;
    else if (JOY_NEW(B_BUTTON))
        task->tKeyboardEvent = INPUT_B_BUTTON;
    else if (JOY_NEW(SELECT_BUTTON))
        task->tKeyboardEvent = INPUT_SELECT;
    else if (JOY_NEW(START_BUTTON))
        task->tKeyboardEvent = INPUT_START;
    else if (JOY_NEW(R_BUTTON))
        task->tKeyboardEvent = INPUT_R_BUTTON;
    else if (JOY_NEW(L_BUTTON))
        task->tKeyboardEvent = INPUT_L_BUTTON;
    else
        HandleDpadMovement(task);
}

static void Input_Override(struct Task *task)
{
    task->tKeyboardEvent = INPUT_NONE;
}

static void TryTurnChineseRow(u8 y, u8 next)
{
    if (y == 0 && IsPageChMode())
    {
        TurnChinesePage(next);
    }
}

static void HandleDpadMovement(struct Task *task)
{
    s16 cursorX;
    s16 cursorY;
    
    GetCursorPos(&cursorX, &cursorY);
    // Get new cursor position
    
    if (JOY_REPEAT(DPAD_UP))
    {
        task->tKeyboardEvent = INPUT_DPAD_UP;
        cursorY--;
    }
    else if (JOY_REPEAT(DPAD_DOWN))
    {
        task->tKeyboardEvent = INPUT_DPAD_DOWN;
        cursorY++;
    }
    else if (JOY_REPEAT(DPAD_LEFT))
    {
        task->tKeyboardEvent = INPUT_DPAD_LEFT;
        cursorX--;
    }
    else if (JOY_REPEAT(DPAD_RIGHT))
    {
        task->tKeyboardEvent = INPUT_DPAD_RIGHT;
        cursorX++;
    }
    else
        return;
    
    if (cursorY < 0)
        cursorY = KBROW_COUNT - 1;
    if (cursorY > KBROW_COUNT - 1)
    {
        cursorY = !CanDPadMoveToTop();
    }
    else if (cursorY == 0 && !CanDPadMoveToTop())
        cursorY = KBROW_COUNT - 1;
    // Wrap cursor position in the X direction
    if (cursorX < 0)
    {
        TryTurnChineseRow(cursorY, FALSE);
        cursorX = GetCurrentPageColumnCountByRow(cursorY) - 1;
    }
    else if (cursorX > GetCurrentPageColumnCountByRow(cursorY) - 1)
    {
        cursorX = 0;
        TryTurnChineseRow(cursorY, TRUE);
    }
    
    SetCursorPos(cursorX, cursorY);
}

#undef tState
#undef tKeyboardEvent
#undef tButtonId

static void DrawNormalTextEntryBox(void)
{
    FillWindowPixelBuffer(sNamingScreen->windows[WIN_TEXT_ENTRY_BOX], PIXEL_FILL(1));
    AddTextPrinterParameterized(sNamingScreen->windows[WIN_TEXT_ENTRY_BOX], FONT_NORMAL, sNamingScreen->template->title, 8, 1, 0, 0);
    PutWindowTilemap(sNamingScreen->windows[WIN_TEXT_ENTRY_BOX]);
}

static void DrawMonTextEntryBox(void)
{
    u8 buffer[64];

    u8 *end = StringCopy(buffer, GetSpeciesName(sNamingScreen->monSpecies));
    WrapFontIdToFit(buffer, end, FONT_NORMAL, 128 - 64);
    StringAppendN(end, sNamingScreen->template->title, 15);
    FillWindowPixelBuffer(sNamingScreen->windows[WIN_TEXT_ENTRY_BOX], PIXEL_FILL(1));
    AddTextPrinterParameterized(sNamingScreen->windows[WIN_TEXT_ENTRY_BOX], FONT_NORMAL, buffer, 8, 1, 0, 0);
    PutWindowTilemap(sNamingScreen->windows[WIN_TEXT_ENTRY_BOX]);
}

static void (*const sDrawTextEntryBoxFuncs[])(void) =
{
    [NAMING_SCREEN_PLAYER]     = DrawNormalTextEntryBox,
    [NAMING_SCREEN_BOX]        = DrawNormalTextEntryBox,
    [NAMING_SCREEN_CAUGHT_MON] = DrawMonTextEntryBox,
    [NAMING_SCREEN_NICKNAME]   = DrawMonTextEntryBox,
    [NAMING_SCREEN_WALDA]      = DrawNormalTextEntryBox,
    [NAMING_SCREEN_CODE]       = DrawNormalTextEntryBox,
};

/*
static void DrawTextEntryBox(void)
{
    sDrawTextEntryBoxFuncs[sNamingScreen->templateNum]();
}
*/

static void DummyGenderIcon(void);
static void DrawGenderIcon(void);

static void (*const sDrawGenderIconFuncs[])(void) =
{
    [FALSE] = DummyGenderIcon,
    [TRUE]  = DrawGenderIcon,
};

static void TryDrawGenderIcon(void)
{
    sDrawGenderIconFuncs[sNamingScreen->template->addGenderIcon]();
}

static void DummyGenderIcon(void)
{

}

static const u8 sGenderColors[2][3] =
{
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_BLUE, TEXT_COLOR_BLUE},
    {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_RED, TEXT_COLOR_RED}
};

static void DrawGenderIcon(void)
{
    u8 text[2];
    bool8 isFemale = FALSE;

    StringCopy(text, gText_MaleSymbol);
    if (sNamingScreen->monGender != MON_GENDERLESS)
    {
        if (sNamingScreen->monGender == MON_FEMALE)
        {
            StringCopy(text, gText_FemaleSymbol);
            isFemale = TRUE;
        }
        AddTextPrinterParameterized3(sNamingScreen->windows[WIN_TEXT_ENTRY], 1, 0x68, 1, sGenderColors[isFemale], -1, text);
    }
}

/*
static u8 GetCharAtKeyboardPos(s16 x, s16 y)
{
    return sKeyboardChars[x + y * KBCOL_COUNT + CurrentPageToKeyboardId() * KBCOL_COUNT * KBROW_COUNT];
}
*/

static u16 GetCharAtKeyboardPos(s16 x, s16 y)
{
    if (y == 0)
    {
        const u8* chars = GetCurrentPageChChars();
        return (chars[x*2] << 8) | chars[x*2+1];
    }
	return gText_NamingScreenKeyboard_Words[CurrentPageToKeyboardId()][y - 1][x];
}

static u8 GetTextEntryPosition(void)
{
    u8 i;

    for (i = 0; i < sNamingScreen->template->maxChars; i++)
    {
        if (sNamingScreen->textBuffer[i] == EOS)
            return i;
    }
    return sNamingScreen->template->maxChars - 1;
}

static u8 GetPreviousTextCaretPosition(void)
{
    s8 i;

    for (i = sNamingScreen->template->maxChars - 1; i > 0; i--)
    {
        if (sNamingScreen->textBuffer[i] != EOS)
            return i;
    }
    return 0;
}

static void DeleteTextCharacter(void)
{
	u8 keyRole = GetKeyRoleAtCursorPos();
	u8 index = GetPreviousTextCaretPosition();
    if (sNamingScreen->chBufferCount > 0)
    {
        sNamingScreen->chBuffer[--sNamingScreen->chBufferCount] = EOS;
        ReDrawChineseTextEntry();
        return;
    }
	
	if (2 == sNamingScreen->gTextMode[index])
	{
		sNamingScreen->textBuffer[index] = 0xFF;
		sNamingScreen->textBuffer[index - 1] = 0xFF;
		sNamingScreen->gTextMode[index] = 0;
		sNamingScreen->gTextMode[--index] = 0;
	}
	else
	{
		sNamingScreen->textBuffer[index] = 0xFF;
		sNamingScreen->gTextMode[index] = 0;
	}
	
	DrawTextEntry();
    CopyBgTilemapBufferToVram(3);
	if (keyRole == KEY_ROLE_CHAR || keyRole == KEY_ROLE_BACKSPACE)
        TryStartButtonFlash(BUTTON_BACK, FALSE, TRUE);
    PlaySE(SE_BALL);
}

// Returns TRUE if the text entry is now full
/*
static bool8 AddTextCharacter(void)
{
    s16 x;
    s16 y;

    GetCursorPos(&x, &y);
    BufferCharacter(GetCharAtKeyboardPos(x, y));
    DrawTextEntry();
    CopyBgTilemapBufferToVram(3);
    PlaySE(SE_SELECT);

    if (GetPreviousTextCaretPosition() != sNamingScreen->template->maxChars - 1)
        return FALSE;
    else
        return TRUE;
}
*/
extern const u8 gText_EmptyString3[];
static const u8* GetCurrentPageChChars()
{
    return &sNamingScreen->curChChars[sNamingScreen->chCharsPos * 2];
}

static bool8 IsPageChMode()
{
    return sNamingScreen->currentPage == KBPAGE_LETTERS_CH;
}

static bool8 CanDPadMoveToTop()
{
    return sNamingScreen->currentPage == KBPAGE_LETTERS_CH && sNamingScreen->chBufferCount > 0;
}

static bool8 AddChCharacterBuffer(u16 ch)
{
    if (sNamingScreen->chBufferCount < sizeof (sNamingScreen->chBuffer)){
        sNamingScreen->chBuffer[sNamingScreen->chBufferCount++] = ch;
        sNamingScreen->chBuffer[sNamingScreen->chBufferCount] = EOS;
        return TRUE;
    }
    return FALSE;
}

static const u8* FindChChars()
{
    u16 i;
    for( i = 0; i < ARRAY_COUNT(gPingYinSerchNode); i++)
    {
        if (StringCompare(sNamingScreen->chBuffer, gPingYinSerchNode[i].key) == 0)
            return gPingYinSerchNode[i].val;
    }
    return gText_EmptyString3;
}

static void CalcNextChars()
{
    u8 i;
    const u8* chars = GetCurrentPageChChars();
    for (i = 0;i < KBCOL_COUNT; i++)
    {
        if (chars[i * 2] == EOS)
            break;
    }
    sNamingScreen->curChRowMax = i;
}

static void DrawChineseTextEntry()
{
    FillWindowPixelBuffer(sNamingScreen->windows[WIN_CH], 0);
    FillWindowPixelBuffer(sNamingScreen->windows[WIN_PINYIN], 0);
    *StringCopyN(gStringVar1, GetCurrentPageChChars(), sNamingScreen->curChRowMax*2) = EOS;
    CopyStrClear(gStringVar1, gStringVar4, COLOUM_PAD - 6);
    AddTextPrinterParameterized3(sNamingScreen->windows[WIN_CH], 1, 4, 0, sKeyboardTextColors, 0, gStringVar4);
    AddTextPrinterParameterized3(sNamingScreen->windows[WIN_PINYIN], 0, 0, 0, sKeyboardTextColors, 0, sNamingScreen->chBuffer);
    PutWindowTilemap(sNamingScreen->windows[WIN_CH]);
    PutWindowTilemap(sNamingScreen->windows[WIN_PINYIN]);
    CopyBgTilemapBufferToVram(sWindowTemplates[WIN_CH].bg);
}

static void ReDrawChineseTextEntry()
{
    sNamingScreen->curChChars = FindChChars();
    sNamingScreen->chCharsPos = 0;
    CalcNextChars();
    DrawChineseTextEntry();
}

static void ClearChiniesTextEntry()
{
    sNamingScreen->chBufferCount = 0;
    sNamingScreen->chBuffer[0] = EOS;
    sNamingScreen->curChChars = gText_EmptyString3;
    sNamingScreen->chCharsPos = 0;
    DrawChineseTextEntry();
}


static void TurnChinesePage(u8 next)
{
    if (next)
    {
        if (sNamingScreen->curChChars != NULL
            && GetCurrentPageChChars()[sNamingScreen->curChRowMax * 2] != EOS)
        {
            sNamingScreen->chCharsPos += sNamingScreen->curChRowMax;
        }
        else
            return;
    }
    else
    {
        if (sNamingScreen->chCharsPos > 0)
        {
            sNamingScreen->chCharsPos -= 12;
        }
        else
            return;
    }
    CalcNextChars();
    DrawChineseTextEntry();
    SetCursorPos(0, 0);
}


static bool8 AddChCharacter(u16 ch)
{
    if (AddChCharacterBuffer(ch))
    {
        ReDrawChineseTextEntry();
        return TRUE;
    }
    return FALSE;
}

static bool8 AddTextCharacter(void)
{
    s16 x;
    s16 y;
	u16 ch;
	u8 index;
	bool8 ismaxch;

    GetCursorPos(&x, &y);
    ch = GetCharAtKeyboardPos(x, y);
    if (y != 0 && IsPageChMode())
    {
        AddChCharacter(ch);
        return FALSE;
    }
    
	index = GetTextEntryPosition();
	if (ch >= 0x100) //是中文
	{
		if (GetPreviousTextCaretPosition() <= sNamingScreen->template->maxChars - 3)
		{
			sNamingScreen->textBuffer[index] = ch >> 8;
			sNamingScreen->textBuffer[index + 1] = ch & 0xFF;
			sNamingScreen->gTextMode[index] = 2;
			sNamingScreen->gTextMode[index + 1] = 2;
			ismaxch = FALSE;
            ClearChiniesTextEntry();
		}
		else ismaxch = TRUE;
	}
	else
	{
		if (GetPreviousTextCaretPosition() != sNamingScreen->template->maxChars - 1)
		{
			sNamingScreen->textBuffer[index] = ch;
			sNamingScreen->gTextMode[index] = 1;
			ismaxch = FALSE;
		}
		else ismaxch = TRUE;
	}
		
	DrawTextEntry();
    PlaySE(SE_SELECT);
	
	return ismaxch;
}

/*
static void BufferCharacter(u16 ch)
{
    u8 index = GetTextEntryPosition();
    sNamingScreen->textBuffer[index] = ch;
}
*/

static void SaveInputText(void)
{
    u8 i;

    for (i = 0; i < sNamingScreen->template->maxChars; i++)
    {
        if (sNamingScreen->textBuffer[i] != CHAR_SPACE && sNamingScreen->textBuffer[i] != EOS)
        {
            StringCopyN(sNamingScreen->destBuffer, sNamingScreen->textBuffer, sNamingScreen->template->maxChars + 1);
            break;
        }
    }
}

static void LoadGfx(void)
{
    LZ77UnCompWram(gNamingScreenBg_Gfx, sNamingScreen->tileBuffer);
    LoadBgTiles(3, sNamingScreen->tileBuffer, 0x2000, 0);
    LZ77UnCompWram(gNamingScreenBg_Rect, sNamingScreen->rectBuffer);
    LZ77UnCompWram(gNamingScreenBg_Rect2, sNamingScreen->rectBuffer2);
    LoadSpriteSheets(sSpriteSheets);
    LoadSpritePalettes(sSpritePalettes);
}

static void CreateHelperTasks(void)
{
    CreateInputHandlerTask();
    CreateButtonFlashTask();
}

static void LoadPalettes(void)
{
    LoadPalette(gNamingScreenBg_Pal, 0, 0x40);
    LoadPalette(sKeyboard_Pal, 0xA0, sizeof(sKeyboard_Pal));
    LoadPalette(GetTextWindowPalette(2), 0xB0, 0x20);
}

static void DrawBgTilemap(u8 bg, const void *src)
{
    CopyToBgTilemapBuffer(bg, src, 0, 0);
}

static void NamingScreen_Dummy(u8 bg, u8 page)
{

}


static void DrawTextEntry(void)
{
	u8 temp[3];
    register u8 i;
	u8 maxChars = sNamingScreen->template->maxChars;
    u16 x = sNamingScreen->inputCharBaseXPos - 96;
	
	FillWindowPixelBuffer(sNamingScreen->windows[WIN_TEXT_ENTRY], PIXEL_FILL(0));
	for (i = 0; i < maxChars; i++)
    {
        if (2 == sNamingScreen->gTextMode[i])
		{
			temp[0] = sNamingScreen->textBuffer[i++];
			temp[1] = sNamingScreen->textBuffer[i];
			temp[2] = 0xFF;
            AddTextPrinterParameterized3(sNamingScreen->windows[WIN_TEXT_ENTRY], 1,  (i - 1) * 8 + x, 1, sKeyboardTextColors, 0xFF, temp);
		}
		else
		{
			temp[0] = sNamingScreen->textBuffer[i];
			temp[1] = 0xFF;
			temp[2] = 0xFF;
            AddTextPrinterParameterized3(sNamingScreen->windows[WIN_TEXT_ENTRY], 1,  i * 8 + x, 1, sKeyboardTextColors, 0xFF, temp);
		}
    }
	
	TryDrawGenderIcon();
    CopyWindowToVram(sNamingScreen->windows[WIN_TEXT_ENTRY], 2);
    PutWindowTilemap(sNamingScreen->windows[WIN_TEXT_ENTRY]);
}


#define IS_CH_BEGIN(currChar) currChar >= 1 && currChar <= 0x1E
static void CopyStrClear(const u8* src, u8* dest, u8 pad)
{
    u8 ch;
    *dest = ch = *src++;
    if (ch == EOS)
        return;
    dest++;
    if (IS_CH_BEGIN(ch))
        *dest++=*src++;
    while (*src != EOS)
    {
        *dest++ = EXT_CTRL_CODE_BEGIN;
        *dest++ = EXT_CTRL_CODE_CLEAR;
        *dest++ = pad;
        *dest++ = ch = *src++;
        if (IS_CH_BEGIN(ch))
            *dest++=*src++;
    }
    *dest = EOS;
}

static void PrintKeyboardKeys(u8 window, u8 page)
{
    FillWindowPixelBuffer(window, 0);
    for (u8 i = 0; i < KBROW_COUNT - 1; i++) // 每一行
    {
        const u8 *row = gText_NamingScreenKeyboard_Words[page][i];
        u8 x = START_X;

        for (u8 j = 0; row[j] != EOS; j++)
        {
            u8 buffer[2] = { row[j], EOS };
            AddTextPrinterParameterized3(
                window, 1, x, i * ROW_HEIGHT,
                sKeyboardTextColors, 0, buffer);
            x += LETTER_WIDTH + LETTER_PAD;
        }
    }
    PutWindowTilemap(window);
}

static void PrintControls(u8 page)
{
    static const ALIGNED(1) u8 gText_Banner_Upper[] = _("拼音{CLEAR 7}ABC{CLEAR 8}1/?{CLEAR 23}删除文字{CLEAR 25}完毕");
    static const ALIGNED(1) u8 gText_Banner_Lower[] = _("拼音{CLEAR 7}abc{CLEAR 8}1/?{CLEAR 23}删除文字{CLEAR 25}完毕");
    AddTextPrinterParameterized3(sNamingScreen->windows[WIN_BANNER], 0, 8, 6, &sKeyboardTextColors[3], 0,
        page == KBPAGE_LETTERS_UPPER ? gText_Banner_Upper : gText_Banner_Lower);
    PutWindowTilemap(sNamingScreen->windows[WIN_BANNER]);
    CopyWindowToVram(sNamingScreen->windows[WIN_BANNER], 3);
}

static void CB2_NamingScreen(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void ResetVHBlank(void)
{
    SetVBlankCallback(NULL);
    SetHBlankCallback(NULL);
}

static void SetVBlank(void)
{
    SetVBlankCallback(VBlankCB_NamingScreen);
}

static void VBlankCB_NamingScreen(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    SetGpuReg(REG_OFFSET_BG1VOFS, sNamingScreen->bg1vOffset);
    SetGpuReg(REG_OFFSET_BG2VOFS, sNamingScreen->bg2vOffset);
    SetGpuReg(REG_OFFSET_BG1CNT, GetGpuReg(REG_OFFSET_BG1CNT) & 0xFFFC);
    SetGpuRegBits(REG_OFFSET_BG1CNT, sNamingScreen->bg1Priority);
    SetGpuReg(REG_OFFSET_BG2CNT, GetGpuReg(REG_OFFSET_BG2CNT) & 0xFFFC);
    SetGpuRegBits(REG_OFFSET_BG2CNT, sNamingScreen->bg2Priority);
}

static void NamingScreen_ShowBgs(void)
{
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);
}


//--------------------------------------------------
// Forward-declared variables
//--------------------------------------------------

// Initial pages below are pointless, they're overwritten with KBPAGE_LETTERS_UPPER in MainState_FadeIn()
static const struct NamingScreenTemplate sPlayerNamingScreenTemplate =
{
    .copyExistingString = FALSE,
    .maxChars = PLAYER_NAME_LENGTH,
    .iconFunction = 1,
    .addGenderIcon = FALSE,
    .initialPage = KBPAGE_LETTERS_CH,
    .unused = 35,
    .title = gText_YourName,
};

static const struct NamingScreenTemplate sPCBoxNamingTemplate =
{
    .copyExistingString = FALSE,
    .maxChars = BOX_NAME_LENGTH,
    .iconFunction = 2,
    .addGenderIcon = FALSE,
    .initialPage = KBPAGE_LETTERS_CH,
    .unused = 19,
    .title = gText_BoxName,
};

static const struct NamingScreenTemplate sMonNamingScreenTemplate =
{
    .copyExistingString = FALSE,
    .maxChars = 10,
    .iconFunction = 3,
    .addGenderIcon = TRUE,
    .initialPage = KBPAGE_LETTERS_CH,
    .unused = 35,
    .title = gText_PkmnsNickname,
};

static const struct NamingScreenTemplate sWaldaWordsScreenTemplate =
{
    .copyExistingString = TRUE,
    .maxChars = 15,
    .iconFunction = 4,
    .addGenderIcon = FALSE,
    .initialPage = KBPAGE_LETTERS_CH,
    .unused = 11,
    .title = gText_TellHimTheWords,
};

static const u8 sText_EnterCode[] = _("请输入代码：");
static const struct NamingScreenTemplate sCodeScreenTemplate = 
{
    .copyExistingString = FALSE,
    .maxChars = CODE_NAME_LENGTH,
    .iconFunction = 5,
    .addGenderIcon = FALSE,
    .initialPage = KBPAGE_LETTERS_UPPER,
    .unused = 35,
    .title = sText_EnterCode,
};

static const struct NamingScreenTemplate *const sNamingScreenTemplates[] =
{
    [NAMING_SCREEN_PLAYER]     = &sPlayerNamingScreenTemplate,
    [NAMING_SCREEN_BOX]        = &sPCBoxNamingTemplate,
    [NAMING_SCREEN_CAUGHT_MON] = &sMonNamingScreenTemplate,
    [NAMING_SCREEN_NICKNAME]   = &sMonNamingScreenTemplate,
    [NAMING_SCREEN_WALDA]      = &sWaldaWordsScreenTemplate,
    [NAMING_SCREEN_CODE]       = &sCodeScreenTemplate,
};

static const struct OamData sOam_8x8 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
};

static const struct OamData sOam_16x16 =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
};



static const struct Subsprite sSubsprites_PCIcon[] =
{
    {
        .x = -8,   
        .y = -12, 
        .shape = SPRITE_SHAPE(16x8),  
        .size = SPRITE_SIZE(16x8), 
        .tileOffset = 0,     
        .priority = 3
    },
    {
        .x = -8,   
        .y = -4,  
        .shape = SPRITE_SHAPE(16x8),  
        .size = SPRITE_SIZE(16x8),
        .tileOffset = 2,     
        .priority = 3
    },
    {
        .x = -8,   
        .y =  4,  
        .shape = SPRITE_SHAPE(16x8),  
        .size = SPRITE_SIZE(16x8), 
        .tileOffset = 4,     
        .priority = 3
    }
};


static const struct SubspriteTable sSubspriteTable_PCIcon[] =
{
    {ARRAY_COUNT(sSubsprites_PCIcon), sSubsprites_PCIcon}
};

static const struct SpriteFrameImage sImageTable_PCIcon[] =
{
    {sPCIconOff_Gfx, sizeof(sPCIconOff_Gfx)},
    {sPCIconOn_Gfx, sizeof(sPCIconOn_Gfx)},
};

static const union AnimCmd sAnim_Loop[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sAnim_CursorSquish[] =
{
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_FRAME(8, 8),
    ANIMCMD_END
};

static const union AnimCmd sAnim_PCIcon[] =
{
    ANIMCMD_FRAME(0, 2),
    ANIMCMD_FRAME(1, 2),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sAnims_Loop[] =
{
    sAnim_Loop
};

static const union AnimCmd *const sAnims_Cursor[] =
{
    sAnim_Loop,
    sAnim_CursorSquish
};

static const union AnimCmd *const sAnims_PCIcon[] =
{
    sAnim_PCIcon
};


static const struct SpriteTemplate sSpriteTemplate_Cursor =
{
    .tileTag = GFXTAG_CURSOR,
    .paletteTag = PALTAG_CURSOR,
    .oam = &sOam_16x16,
    .anims = sAnims_Cursor,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Cursor
};

static const struct SpriteTemplate sSpriteTemplate_InputArrow =
{
    .tileTag = GFXTAG_INPUT_ARROW,
    .paletteTag = PALTAG_PAGE_SWAP_OTHERS,
    .oam = &sOam_8x8,
    .anims = sAnims_Loop,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_InputArrow
};

static const struct SpriteTemplate sSpriteTemplate_Underscore =
{
    .tileTag = GFXTAG_UNDERSCORE,
    .paletteTag = PALTAG_PAGE_SWAP_OTHERS,
    .oam = &sOam_8x8,
    .anims = sAnims_Loop,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Underscore
};

static const struct SpriteTemplate sSpriteTemplate_PCIcon =
{
    .tileTag = 0xFFFF,
    .paletteTag = PALTAG_PC_ICON,
    .oam = &sOam_8x8,
    .anims = sAnims_PCIcon,
    .images = sImageTable_PCIcon,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};




static const struct SpriteSheet sSpriteSheets[] =
{
    {gNamingScreenBackButton_Gfx,     0x1E0,  GFXTAG_BACK_BUTTON},
    {gNamingScreenOKButton_Gfx,       0x1E0,  GFXTAG_OK_BUTTON},
    {gNamingScreenPageSwapFrame_Gfx,  0x280,  GFXTAG_PAGE_SWAP_FRAME},
    {gNamingScreenPageSwapButton_Gfx, 0x100,  GFXTAG_PAGE_SWAP_BUTTON},
    {gNamingScreenPageSwapUpper_Gfx,  0x060,  GFXTAG_PAGE_SWAP_UPPER},
    {gNamingScreenPageSwapLower_Gfx,  0x060,  GFXTAG_PAGE_SWAP_LOWER},
    {gNamingScreenPageSwapOthers_Gfx, 0x060,  GFXTAG_PAGE_SWAP_OTHERS},
    {gNamingScreenCursor_Gfx,         0x080,  GFXTAG_CURSOR},
    {gNamingScreenCursorSquished_Gfx, 0x080,  GFXTAG_CURSOR_SQUISHED},
    {gNamingScreenCursorFilled_Gfx,   0x080,  GFXTAG_CURSOR_FILLED},
    {gNamingScreenInputArrow_Gfx,     0x020,  GFXTAG_INPUT_ARROW},
    {gNamingScreenUnderscore_Gfx,     0x020,  GFXTAG_UNDERSCORE},
    {}
};

static const u16 gUnderscore_Pal[] =  INCBIN_U16("graphics/naming_screen/underscore.gbapal");

static const struct SpritePalette sSpritePalettes[] =
{
    {gNamingScreenMenu_Pal[0], PALTAG_PC_ICON},
    {gNamingScreenMenu_Pal[1], PALTAG_PAGE_SWAP_UPPER},
    {gNamingScreenMenu_Pal[2], PALTAG_PAGE_SWAP_LOWER},
    {gNamingScreenMenu_Pal[3], PALTAG_PAGE_SWAP_OTHERS},
    {gNamingScreenMenu_Pal[4], PALTAG_PAGE_SWAP},
    {gNamingScreenMenu_Pal[5], PALTAG_CURSOR},
    {gNamingScreenMenu_Pal[4], PALTAG_BACK_BUTTON},
    {gNamingScreenMenu_Pal[4], PALTAG_OK_BUTTON},
    {}
};


