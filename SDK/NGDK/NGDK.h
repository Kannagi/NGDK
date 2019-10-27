#ifndef __NGDK__
#define __NGDK__

//10/10/2019 - 20/10/2019
#define NGDK_VERSION 70
/*
Function Bullter

initBuller20
40
100

UpdateTile20
UpdateTile40
UpdateTile100


*/
/*
	To do list :
	-collision/hitbox 0.8
	-Yorder(Zorder) 0.9
	-sound driver 1.0

*/
#define NG_NOP "nop\n	"
#define NG_NOP3 "nop\n	nop\n	nop\n	"
//#define NG_NOP "\n	"
//#define NG_NOP3 "\n	"
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

#define ftoi2(ft2) (u16)(ft2*4)
#define ftoi4(ft4) (u16)(ft4*16)
#define ftoi7(ft7) (u16)(ft7*128)

#define ftol2(ft2) (u32)(ft2*4)
#define ftol4(ft4) (u32)(ft4*16)
#define ftol7(ft7) (u32)(ft4*128)
#define ftol8(ft8) (u32)(ft8*256)
#define ftol12(ft12) (u32)(ft12*4096)
#define ftol16(ft16) (u32)(ft16*65536)

#define itoi2(it2) ( it2 >>2)
#define itoi4(it4) ( it4 >>4)
#define itoi7(it7) ( it7 >>7)
#define itoi8(it8) ( it8 >>8)
#define itoi12(it12) ( it12 >>12)
#define itoi16(it16) ( it16 >>16)

#define NG_MAX_SPR 128
#define NG_MAX_BULLET 256
#define NG_MAX_BULLET_PILE 256
#define NG_JOYPAD_INIT {0,0,0,0,0,0,0,0}

#define FIXPAL(_x) (_x<<12)
#define NG_FLAG(_pal,_tile,_anim,_flip) (_pal<<8 | _tile<<4 | _anim<<2 | _flip)
#define NG_ZOOM(_x,_y) ( (_y<<8) | (_x) )
#define NG_COLOR(_r,_g,_b) ( (_r << 8) | ( (_g) << 4) | _b )

#define NG_BUTTON_DOWN   1
#define NG_BUTTON_DOWNW  2
#define NG_BUTTON_UP     3

/*
enum
{
	NG_ENUM = 0x01,

};
*/

typedef struct
{
    u16 A,B,C,D,UP,RIGHT,LEFT,DOWN;
}NG_Joypads;

typedef struct
{
    u16 id,zoom,y,h;
    u16 x,w,tile,flag;
}NG_Sprites;

typedef struct
{
    u16 y,x,tile,flag;
    u16 vx,vy,py,px;
}NG_Bullets;

void NG_SpriteClearVRAM(void);
void NG_WaitVBlank(void);
void NG_BGColor(u16 color);
u16 NG_CPU();
void NG_Cycle(u16 x,u16 y);

u16 NG_Joypad1(NG_Joypads *pad);
u16 NG_Joypad2(NG_Joypads *pad);

void NG_ClearFix(void);
void NG_FixPal(u16 pal);

void NG_LoadPalN(const void *palette,u16 address,u16 num);
void NG_LoadPal(const void *palette,u16 address);

void NG_PrintFix(u16 val,u16 x,u16 y);
void NG_PrintCPU(u16 x,u16 y);
void NG_PrintFreeMem(u16 x,u16 y);
void NG_Print(const void *str,u16 x,u16 y);

void NG_Prints8(s8 val,u16 x,u16 y);
void NG_Printu8(u8 val,u16 x,u16 y);
void NG_Printh8(u8 val,u16 x,u16 y);

void NG_Prints16(s16 val,u16 x,u16 y);
void NG_Printu16(u16 val,u16 x,u16 y);
void NG_Printh16(u16 val,u16 x,u16 y);

void NG_Prints32(u32 val,u16 x,u16 y);
void NG_Printu32(u32 val,u16 x,u16 y);
void NG_Printh32(u32 val,u16 x,u16 y);


void NG_SpriteSetID(u16 id);
u16 NG_SpriteGetID(void);
void NG_SpriteUpdateN(u16 n);

void NG_SpriteInit(u16 i,u16 x,u16 y,u16 w,u16 h,u16 tile,u16 flag);
void NG_SpriteUpdateInit(u16 i);
void NG_SpriteUpdate(u16 i);
void NG_SpriteUpdateTile(u16 i);
void NG_SpriteUpdateZoom(u16 i);

void NG_SpriteInitHicolor(u16 i,u16 x,u16 y,u16 w,u16 h,u16 tile,u16 flag);
void NG_SpriteUpdateInitHicolor(u16 i);
void NG_SpriteUpdateZoomHicolor(u16 i);
void NG_SpriteUpdateTileHicolor(u16 i);
void NG_SpriteUpdateHicolor(u16 i);

void NG_SpriteUpdateMapHorizontal(const void *tile,u16 i,u16 offset);
void NG_SpriteUpdateMapVertical(const void *tile,u16 i,u16 offset);

void NG_BGWidthSet(u16 w);
u16 NG_BGWidthGet();
void NG_BGScrollx(const void *data,u16 w,u16 i);

extern NG_Sprites NG_Sprite[NG_MAX_SPR];
extern NG_Bullets NG_Bullet[NG_MAX_BULLET];
extern u16 NG_Bullet_Pile1[NG_MAX_BULLET_PILE];
extern u16 NG_Bullet_Pile2[NG_MAX_BULLET_PILE];

typedef struct
{
    int act,old,i,itmp,end;
}NG_Anim;

void NG_Anim_Play_Fast(NG_Anim *anim,u16 cadence,u16 n);
u16 NG_Anim_Play(NG_Anim *anim,u16 cadence,u16 n);
void NG_Anim_Init(NG_Anim *anim);

void NG_Map_Decompress(const void *src,void *dst,u16 n);

void NG_BulletUpdate(u16 id,u16 n);
void NG_BulletUpdateTile(u16 id,u16 n);

void NG_BulletPosition(u16 pile,u16 offset,u16 n);
void NG_BulletUpdatePosition(u16 id,u16 n);

void NG_SetYM2612(u8 command);
#endif



