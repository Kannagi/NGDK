#define NG_WaitVBlank NG_Wait_VBlank

#define NG_BGColor NG_Backdrop_Color_Set

#define NG_Joypad1 NG_Joypad1_Read
#define NG_Joypad2 NG_Joypad2_Read

#define NG_LoadPal NG_Palette_Load
#define NG_LoadPalN NG_Palette_Load_N

#define NG_ClearFix NG_Fix_Clear
#define NG_FixPal NG_Fix_Palette
#define NG_PrintFix NG_Fix_Print_Tile
#define NG_PrintCPU NG_Fix_Print_CPU
#define NG_Cycle NG_Fix_Print_Cycles
#define NG_PrintFreeMem NG_Fix_Print_Free_Mem
#define NG_Print NG_Fix_Print_String

#define NG_Prints8 NG_Fix_Print_s8
#define NG_Printu8 NG_Fix_Print_u8
#define NG_Printh8 NG_Fix_Print_h8

#define NG_Prints16 NG_Fix_Print_s16
#define NG_Printu16 NG_Fix_Print_u16
#define NG_Printh16 NG_Fix_Print_h16

#define NG_Prints32 NG_Fix_Print_s32
#define NG_Printu32 NG_Fix_Print_u32
#define NG_Printh32 NG_Fix_Print_h32

#define NG_SpriteClearVRAM NG_Sprite_Clear_VRAM
#define NG_SpriteInit NG_Sprite_Init
#define NG_SpriteSetID NG_Sprite_VRAM_ID_Set
#define NG_SpriteGetID NG_Sprite_VRAM_ID_Get
#define NG_SpriteUpdateInit NG_Sprite_VRAM_Init
#define NG_SpriteUpdate NG_Sprite_Update
#define NG_SpriteUpdateN NG_Sprite_Update_N
#define NG_SpriteUpdateTile NG_Sprite_Tiles_Update
#define NG_SpriteUpdateZoom NG_Sprite_Zoom_Update

#define NG_SpriteInitHiColor NG_Sprite_HiColor_Init
#define NG_SpriteVRAM_InitHiColor NG_Sprite_HiColor_VRAM_Init
#define NG_SpriteUpdateHiColor NG_Sprite_HiColor_Update
#define NG_SpriteUpdateTilesHiColor NG_Sprite_HiColor_Tiles_Update
#define NG_SpriteUpdateZoomHiColor NG_Sprite_HiColor_Zoom_Update

#define NG_BGWidthSet NG_Background_Width_Set
#define NG_BGWidthGet NG_Background_Width_Get

#define NG_SpriteUpdateMapVertical(data, col, id) (NG_Background_Vertical_Update(id, col, data))
#define NG_SpriteUpdateMapHorizontal(data, col, id) (NG_Background_Horizontal_Update(id, col, data))
#define NG_BGScrollx(data, offset, id) (NG_Background_Scroll_X(id, offset, data))

#define NG_SetYM2610 NG_YM2610_Command_Send
#define NG_GetYM2610 NG_YM2610_Get

//#define NG_Anim_Init 
//#define NG_Anim_Play_Fast 
//#define NG_Anim_Play 
