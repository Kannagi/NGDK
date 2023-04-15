#ifndef __NGDK_ARG__
#define __NGDK_ARG__

volatile u8 NG_arg1_u8;
volatile u8 NG_arg2_u8;
volatile u8 NG_arg3_u8;
volatile u8 NG_arg4_u8;

volatile u16 NG_arg1_u16;
volatile u16 NG_arg2_u16;
volatile u16 NG_arg3_u16;
volatile u16 NG_arg4_u16;

volatile u32 NG_arg1_u32;
volatile u32 NG_arg2_u32;
volatile u32 NG_arg3_u32;
volatile u32 NG_arg4_u32;

NG_Sprite NG_Sprites[NG_MAX_SPR];
u16* NG_tile_array_ptr;
u16 NG_VRAM_sprite_id;
u16 NG_PAL_ID;

u16 NG_BG_W;
#endif
