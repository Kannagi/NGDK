#ifndef __NGDK_ARG__
#define __NGDK_ARG__

u8 NG_arg1_u8;
u8 NG_arg2_u8;
u8 NG_arg3_u8;
u8 NG_arg4_u8;

u16 NG_arg1_u16;
u16 NG_arg2_u16;
u16 NG_arg3_u16;
u16 NG_arg4_u16;

u32 NG_arg1_u32;
u32 NG_arg2_u32;
u32 NG_arg3_u32;
u32 NG_arg4_u32;


NG_Sprites NG_Sprite[NG_MAX_SPR];
NG_Bullets NG_Bullet[NG_MAX_BULLET];
u16 NG_Bullet_Pile1[NG_MAX_BULLET_PILE];
u16 NG_Bullet_Pile2[NG_MAX_BULLET_PILE];
u16 NG_ID;
u16 NG_PAL_ID;

u16 NG_BG_W;
#endif
