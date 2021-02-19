/**
 * @file NGDK.h
 * @brief Neo Geo Development Kit (NGDK)
 * @author Kannagi
 * @author Antoine Gohin
 * @version 0.71
 * @date 10/2019 - 02/2021
 *
 * TODO LIST
 * - Sound driver
 * - Handle multi directionnal scrolling and "big" maps
 * - Improve TMX files import
 * - Collision / hitbox
 * - Y order / Z order
 */

#ifndef __NGDK__
#define __NGDK__

#define NGDK_VERSION 71

// Legacy functions names support
#include "defines.h"

#define NG_NOP "nop\n	"
#define NG_NOP3 "nop\n	nop\n	nop\n	"
//#define NG_NOP " "
//#define NG_NOP3 " "
#define RW_REGISTER_U8(REG) 	*((volatile unsigned char  *)(REG))
#define RW_REGISTER_U16(REG) 	*((volatile unsigned short *)(REG))
#define RW_REGISTER_U32(REG) 	*((volatile unsigned int   *)(REG))
#define RW_REGISTER_U64(REG) 	*((volatile unsigned long  *)(REG))

#define u8  unsigned char
#define u16 unsigned short
#define u32 unsigned int

#define s8  signed char
#define s16 signed short
#define s32 signed int

/**
 * @brief Converts double to fix16 14.2.
 *
 */
#define ftoi2(ft2) (u16)((ft2) * 4)

/**
 * @brief Converts double to fix16 12.4.
 *
 */
#define ftoi4(ft4) (u16)((ft4) * 16)

/**
 * @brief Converts double to fix16 9.7.
 *
 */
#define ftoi7(ft7) (u16)((ft7) * 128)

/**
 * @brief Converts double to fix16 8.8.
 *
 */
#define ftoi8(ft8) (u16)((ft8) * 256)

/**
 * @brief Converts double to fix32 30.2.
 *
 */
#define ftol2(ft2) (u32)((ft2) * 4)

/**
 * @brief Converts double to fix32 30.4.
 *
 */
#define ftol4(ft4) (u32)((ft4) * 16)

/**
 * @brief Converts double to fix32 25.7.
 *
 */
#define ftol7(ft7) (u32)((ft7) * 128)

/**
 * @brief Converts double to fix32 24.8.
 *
 */
#define ftol8(ft8) (u32)((ft8) * 256)

/**
 * @brief Converts double to fix32 20.12.
 *
 */
#define ftol12(ft12) (u32)((ft12) * 4096)

/**
 * @brief Converts double to fix32 16.16.
 *
 */
#define ftol16(ft16) (u32)((ft16) * 65536)

/**
 * @brief Converts fix xx.2 to unsigned.
 *
 */
#define itoi2(it2) ((it2) >> 2)

/**
 * @brief Converts fix xx.4 to unsigned.
 *
 */
#define itoi4(it4) ((it4) >> 4)

/**
 * @brief Converts fix xx.7 to unsigned.
 *
 */
#define itoi7(it7) ((it7) >> 7)

/**
 * @brief Converts fix xx.8 to unsigned.
 *
 */
#define itoi8(it8) ((it8) >> 8)

/**
 * @brief Converts fix xx.12 to unsigned.
 *
 */
#define itoi12(it12) ((it12) >> 12)

/**
 * @brief Converts fix xx.16 to unsigned.
 *
 */
#define itoi16(it16) ((it16) >> 16)

#define NG_MAX_SPR 380
#define NG_JOYPAD_INIT {0, 0, 0, 0, 0, 0, 0, 0}

/**
 * @brief Generates shifted index for fix palette.
 *
 */
#define NG_FIX_PAL(_x) (_x << 12)

/**
 * @brief Generates sprite flags from arguments.
 *
 * @param _pal Palette index to use (8bits)
 * @param _tile Start tile MSBs (4bits)
 * @param _anim Auto-animation flag (2bits)
 * @param _flip H/V flip flags (2bits)
 */
#define NG_SPR_FLAGS(_pal, _tile, _anim, _flip) ((_pal << 8) | (_tile << 4) | (_anim << 2) | _flip)

#define NG_ZOOM(_x, _y) ( (_y << 8) | (_x) )

/**
 * @brief Generates RGB value.
 *
 * @param _r Red value
 * @param _g Green value
 * @param _b Blue value
 */
#define NG_COLOR(_r, _g, _b) ( (_r << 8) | ( (_g) << 4) | _b )

#define NG_ANIM_RUNNING	0
#define NG_ANIM_DONE	1
#define NG_ANIM_UPDATED	2

#define NG_BUTTON_UP	0x01
#define NG_BUTTON_DOWN	0x02
#define NG_BUTTON_LEFT	0x04
#define NG_BUTTON_RIGHT	0x08
#define NG_BUTTON_A		0x10
#define NG_BUTTON_B		0x20
#define NG_BUTTON_C		0x40
#define NG_BUTTON_D		0x80

#define NG_BUTTON_NOT_PRESSED	0
#define NG_BUTTON_PRESSED		1
#define NG_BUTTON_HOLD			2
#define NG_BUTTON_RELEASED		3

/**
 * @brief Joypad structure.
 *
 * @param state Joypad current state, one bit per button, on/off, use NG_BUTTON_x to filter
 * @param UP Joypad UP state
 * @param DOWN Joypad DOWN state
 * @param LEFT Joypad LEFT state
 * @param RIGHT Joypad RIGHT state
 * @param A Joypad A state
 * @param B Joypad B state
 * @param C Joypad C state
 * @param D Joypad D state
 */
typedef struct
{
	u16 state;
	u16 UP, DOWN, LEFT, RIGHT, A, B, C, D;
} NG_joypad;

/**
 * @brief Sprite structure.
 *
 * @param VRAM_id VRAM sprite id
 * @param zoom Zoom coeff
 * @param y Y position
 * @param height Sprite height in tiles
 * @param x X position
 * @param width Sprite width in tiles
 * @param tile First tile index
 * @param flags Sprite flags/attributes (palette, tile MSBs, auto-anim, H/V flip)
 */
typedef struct
{
	u16 VRAM_id, zoom, y, height;
	u16 x, width, tile, flags;
} NG_Sprite;

/**
 * @brief Sprite animation structure.
 *
 * @param current Current animation id		| used to reset counters
 * @param previous Previous animation id	| when changing animation
 * @param frame Current frame
 * @param frameCounter Frame counter
 * @param status Animation status
 */
typedef struct
{
	int current, previous, frame, frameCounter, status;
}NG_Animation;

extern NG_Sprite NG_Sprites[NG_MAX_SPR];

/**
 * @brief Waits for next VBLANK to begin.
 *
 */
void NG_Wait_VBlank(void);

/**
 * @brief Sets backdrop color.
 *
 * @param color Color (NG_COLOR macro can be used)
 */
void NG_Backdrop_Color_Set(u16 color);

/**
 * @brief Reads joypad 1.
 *
 * @param pad NG_joypad structure to update
 * @return u16 Joypad current state
 */
u16 NG_Joypad1_Read(NG_joypad *pad);

/**
 * @brief Reads joypad 2.
 *
 * @param pad NG_joypad structure to update
 * @return u16 Joypad current state
 */
u16 NG_Joypad2_Read(NG_joypad *pad);

/**
 * @brief Loads a palette.
 *
 * @param palette Palette pointer
 * @param address Palette index (0-255)
 */
void NG_Palette_Load(const void *palette, u16 index);

/**
 * @brief Loads a palette multiple times.
 *
 * @param palette Palette pointer
 * @param address Palette index (0-255), starting point
 * @param n Number of times we need to load the palette
 */
void NG_Palette_Load_N(const void *palette, u16 index, u16 n);

/**
 * @brief Calls BIOSF_CLEARFIX.
 *        Clears all the fix layer with tile # $FF, palette 0. The leftmost and rightmost columns are cleared with tile # $20, palette 0.
 *        In the default fix tilesets, tile # $FF is filled with color 0 (transparent). Tile # $20 is opaque (used for masking).
 *
 */
void NG_Fix_Clear(void);

/**
 * @brief Selects palette to use for fix layer.
 *
 * @param pal Palette index
 */
void NG_Fix_Palette(u16 pal);

/**
 * @brief Prints a specific tile on fix layer.
 *
 * @param id Tile id
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
void NG_Fix_Print_Tile(u16 id, u16 x, u16 y);

/**
 * @brief Prints CPU usage in percentage on fix layer.
 *
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
void NG_Fix_Print_CPU(u16 x, u16 y);

/**
 * @brief Prints number of executed cycles (u16) on fix layer.
 *
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
void NG_Fix_Print_Cycles(u16 x, u16 y);

/**
 * @brief Prints free memory remaining on fix layer.
 *
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
void NG_Fix_Print_Free_Mem(u16 x, u16 y);

/**
 * @brief Prints a string on fix layer.
 *
 * @param str String
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
void NG_Fix_Print_String(const void *str, u16 x, u16 y);

/**
 * @brief Prints an 8bit signed value on fix layer.
 *
 * @param val 8bit signed value
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
void NG_Fix_Print_s8(s8 val, u16 x, u16 y);

/**
 * @brief Prints an 8bit unsigned value on fix layer.
 *
 * @param val 8bit unsigned value
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
void NG_Fix_Print_u8(u8 val, u16 x, u16 y);

/**
 * @brief Prints an 8bit unsigned value in hex format on fix layer.
 *
 * @param val 8bit unsigned value
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
void NG_Fix_Print_h8(u8 val, u16 x, u16 y);

/**
 * @brief Prints 16bit signed value on fix layer.
 *
 * @param val 16 bit signed value
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
void NG_Fix_Print_s16(s16 val, u16 x, u16 y);

/**
 * @brief Prints 16bit unsigned value on fix layer.
 *
 * @param val 16 bit unsigned value
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
void NG_Fix_Print_u16(u16 val, u16 x, u16 y);

/**
 * @brief Prints 16bit unsigned value in hex format on fix layer.
 *
 * @param val 16 bit unsigned value
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
void NG_Fix_Print_h16(u16 val, u16 x, u16 y);

/**
 * @brief Prints 32bit signed value on fix layer.
 *
 * @param val 32 bit signed value
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
//void NG_Fix_Print_s32(u32 val, u16 x, u16 y);

/**
 * @brief Prints 32bit unsigned value on fix layer.
 *
 * @param val 32 bit unsigned value
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
//void NG_Fix_Print_u32(u32 val, u16 x, u16 y);

/**
 * @brief Prints 32bit unsigned value in hex format on fix layer.
 *
 * @param val 32 bit unsigned value
 * @param x X position (tile 0-39)
 * @param y Y position (tile 0-27)
 */
void NG_Fix_Print_h32(u32 val, u16 x, u16 y);

/**
 * @brief Calls BIOSF_CLEARSPR:
 *        All shrinking values are set to max (all SCB2 is set to $0FFF).
 *        All sprite heights are set to 0 and Y positions set to 496 (all SCB3 is set to $0000).
 *        All sprite X positions are set to 380 (all SCB4 is set to $BE00).
 *        The map of sprite #0 is set to tiles # $FF (SCB1 $0000~$0040 to $00FF,$0000).
 *
 */
void NG_Sprite_Clear_VRAM(void);

/**
 * @brief Sprite initialization.
 *
 * @param id Sprite id for NG_Sprites array (0-383)
 * @param x X position
 * @param y Y position
 * @param width Width in tiles
 * @param height Height in tiles
 * @param tile First tile ID
 * @param flags Sprite flags (palette, tile MSBs, auto-animation, V flip, H flip)
 */
void NG_Sprite_Init(u16 id, u16 x, u16 y, u16 width, u16 height, u16 tile, u16 flags);

/**
 * @brief Sets VRAM sprite id as starting point for NG_Sprite_VRAM_Init.
 *
 * @param id VRAM sprite id
 */
void NG_Sprite_VRAM_ID_Set(u16 id);

/**
 * @brief Gets current VRAM sprite id.
 *
 * @return u16 VRAM sprite id
 */
u16 NG_Sprite_VRAM_ID_Get(void);

/**
 * @brief Registers sprite in VRAM.
 *        VRAM id will be set in NG_Sprites[id] properties.
 *        Creates tilemap and update sprite in VRAM.
 *
 * @param id Sprite id (NG_Sprites)
 */
void NG_Sprite_VRAM_Init(u16 id);

/**
 * @brief Updates a sprite.
 *        Copies zoom, Y position, sprite size, X position to VRAM.
 *
 * @param id Sprite id (NG_Sprites)
 */
void NG_Sprite_Update(u16 id);

/**
 * @brief Updates sprite 0 to sprite n (NG_Sprites).
 *        Copies zoom, Y position, sprite size, X position to VRAM.
 *
 * @param n Number of sprites to update
 */
void NG_Sprite_Update_N(u16 n);

/**
 * @brief Updates a sprite tiles data to VRAM.
 *
 * @param id Sprite id (NG_Sprites)
 */
void NG_Sprite_Tiles_Update(u16 id);

/**
 * @brief Updates a sprite zoom to VRAM.
 *
 * @param id Sprite id (NG_Sprites)
 */
void NG_Sprite_Zoom_Update(u16 id);

/**
 * @brief Hi-Color sprite initialization.
 *
 * @param id Sprite id for NG_Sprites array (0-383)
 * @param x X position
 * @param y Y position
 * @param width Width in tiles
 * @param height Height in tiles
 * @param tile First tile ID
 * @param flags Sprite flags (palette, tile MSBs, auto-animation, V flip, H flip)
 */
void NG_Sprite_HiColor_Init(u16 id, u16 x, u16 y, u16 width, u16 height, u16 tile, u16 flags);

/**
 * @brief Registers a Hi-Color sprite in VRAM.
 *        VRAM id will be set in NG_Sprites[id].VRAM_id properties.
 *        Creates tilemap and update sprite in VRAM.
 *
 * @param id Sprite id (NG_Sprites)
 */
void NG_Sprite_HiColor_VRAM_Init(u16 id);

/**
 * @brief Updates a Hi-Color sprite.
 *        Copies zoom, Y position, sprite size, X position to VRAM.
 *
 * @param id Sprite id (NG_Sprites)
 */
void NG_Sprite_HiColor_Update(u16 id);

/**
 * @brief Updates a Hi-Color sprite tiles data to VRAM.
 *
 * @param id Sprite id (NG_Sprites)
 */
void NG_Sprite_HiColor_Tiles_Update(u16 id);

/**
 * @brief Updates a Hi-Color sprite zoom to VRAM.
 *
 * @param id Sprite id (NG_Sprites)
 */
void NG_Sprite_HiColor_Zoom_Update(u16 id);

/**
 * @brief Sets background width.
 *
 * @param width Width in tiles
 */
void NG_Background_Width_Set(u16 width);

/**
 * @brief Returns background width.
 *
 * @return u16 Width in tiles
 */
u16 NG_Background_Width_Get();

/**
 * @brief Updates background column.
 *
 * @param id Background sprite id for NG_Sprites array (0-383)
 * @param col Column index (0-19)
 * @param data Pointer to map tiles
 */
void NG_Background_Vertical_Update(u16 id, u16 col, void *data);

/**
 * @brief Updates background row.
 *
 * @param id Background sprite id for NG_Sprites array (0-383)
 * @param row Row index (0-13)
 * @param data Pointer to map tiles
 */
void NG_Background_Horizontal_Update(u16 id, u16 row, void *data);

/**
 * @brief Updates background sprite tilemap depending on offset passed.
 *
 * @param id Background sprite id for NG_Sprites array (0-383)
 * @param offset Offset from current row, usually -1 (left) or +20 (right)
 * @param data Pointer to map tiles
 */
void NG_Background_Scroll_X(u16 id, u16 offset, void *data);

/**
 * @brief Sets animation properties to 0.
 *
 * @param anim Pointer to animation
 */
void NG_Anim_Init(NG_Animation *anim);

/**
 * @brief Updates animation.
 *
 * @param anim Pointer to animation
 * @param frameWait Number of frames to wait before updating frame index
 * @param frameMax Number of frames for the animation
 */
void NG_Anim_Play_Fast(NG_Animation *anim, u16 frameWait, u16 frameMax);

/**
 * @brief Updates animation, and returns its status.
 *
 * @param anim Pointer to animation
 * @param frameWait Number of frames to wait before updating frame index
 * @param frameMax Number of frames for the animation
 * @return u16 Animation status (see NG_ANIM_xxx)
 */
u16 NG_Anim_Play(NG_Animation *anim, u16 frameWait, u16 frameMax);

/**
 * @brief Sends command to YM2610.
 *
 * @param command Command to send
 */
void NG_Set_YM2610(u8 command);

/**
 * @brief Returns YM2610 status. (TBC?)
 *
 * @return u8 YM2610 status
 */
u8 NG_Get_YM2610();

#endif
