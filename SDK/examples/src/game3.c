#include <NGDK.h>
#include "asm.h"

#define NG_MAX_BULLET 256
#define NG_MAX_BULLET_PILE 256

typedef struct
{
	u16 y, x, tile, flag;
	u16 vx, vy, py, px;
} NG_Bullet;

NG_Bullet NG_Bullets[NG_MAX_BULLET];
u16 NG_Bullet_Pile1[NG_MAX_BULLET_PILE];
u16 NG_Bullet_Pile2[NG_MAX_BULLET_PILE];

u16 NG_arg1_u16;
u16 NG_arg2_u16;

u32 NG_arg1_u32;
u32 NG_arg2_u32;

void NG_BulletUpdate(u16 id, u16 n);
void NG_BulletUpdateTile(u16 id, u16 n);

void NG_BulletPosition(u16 pile, u16 offset, u16 n);
void NG_BulletUpdatePosition(u16 id, u16 n);

#include "data_ext.h"

extern const u16 palette_ng[32];

void offset3(void)
{
	NG_Sprite_Update_N(4);
}

void __attribute__((optimize("-O3"), noinline)) game3(void )
{
	NG_joypad joypad1 = NG_JOYPAD_INIT; //Init Joypad
	NG_Sprite_Clear_VRAM(); //Clear VRAM Sprite
	NG_Fix_Clear(); //Clear Fix

	NG_Backdrop_Color_Set(NG_COLOR(7, 7, 7)); //Backdrop color R, G, B (0-15)

	//Load Palette (FIX 0-15)
	NG_Palette_Load(palette_ng, 0); //Load palette, palette index
	RW_REGISTER_U16(0x3C0006) = 0x700;

	//MAP
	NG_Palette_Load(BGD_pal_data, 16);

	NG_Palette_Load(ship_pal_data, 0x80);
	NG_Palette_Load(booster_pal_data, 0x81);
	NG_Palette_Load(bullet_pal_data, 0x82);
	NG_Palette_Load(bullet2_pal_data, 0x83);
	NG_Palette_Load_N(Boss_pal_data, 0x84, 1);

	//-----------fix--------------
	NG_Fix_Palette(NG_FIX_PAL(0)); //Select Pal for Print
	NG_Fix_Print_String("Dobonpachi !", 12, 0); //Print text, str, X pos, Y pos
	NG_Fix_Print_String("CPU:", 29, 0);
	NG_Fix_Print_u16(NGDK_VERSION, 0, 0); //Print variable u16, X pos, Y pos
	//NG_Fix_Print_u16(NG_MAX_SPR, 0, 1);

	//NG_Fix_Print_Free_Mem(0, 5);

	//-----------init sprite--------------
	NG_Sprite_Init(0, 0, 0, 20, 32, BGD_index, 0x1000);  //BG1
	NG_Sprite_Init(1, 130, 130, 2, 2, ship_index, NG_SPR_FLAGS(0x80, 0, 0, 0)); //Sprite, w, h, tile, flag(pal, tileMSB, auto-anim, flip)
	NG_Sprite_Init(2, 130, 130, 2, 1, booster_index, NG_SPR_FLAGS(0x81, 0, 0, 0));

	NG_Sprite_HiColor_Init(3, 40, 0, 14, 8, Boss_index, NG_SPR_FLAGS(0x84, 0, 0, 0));

	//----Init Sprite VRAM------------
	NG_Sprite_VRAM_ID_Set(0x00);
	NG_Sprite_VRAM_Init(0); //BG1

	NG_Sprite_VRAM_Init(3);
	NG_Sprite_VRAM_Init(4);

	NG_Sprite_VRAM_Init(1);
	NG_Sprite_VRAM_Init(2);
	NG_Sprite_Update_N(4); //Update position X, Y, MetaSPR, Zoom XY, 0-N

	//NG_Sprite_VRAM_ID_Get gives real ID, max 380
	//NG_Fix_Print_u16(NG_Sprite_VRAM_ID_Get(), 0, 2);
	u16 i;

	for(i = 0;i < 40;i++)
	{
		NG_Bullets[i].x = 0;
		NG_Bullets[i].y = ftoi7(256);
		NG_Bullets[i].tile = bullet_index;
		NG_Bullets[i].flag = NG_SPR_FLAGS(0x82, 0, 0, 0);
		NG_Bullets[i].vx = 0;
		NG_Bullets[i].vy = 0;
	}

	for(i = 40;i < NG_MAX_BULLET;i++)
	{
		NG_Bullets[i].x = 0;
		NG_Bullets[i].y = ftoi7(256);
		NG_Bullets[i].tile = bullet2_index;
		NG_Bullets[i].flag = NG_SPR_FLAGS(0x83, 0, 1, 0);
		NG_Bullets[i].vx = 0;
		NG_Bullets[i].vy = 0;
	}

	u16 animb = 0;
	u16 nbullet = 31;
	u16 idbullet = 124;
	NG_BulletUpdateTile(idbullet, nbullet);

	NG_Wait_VBlank(); //Wait VBlank
	u16 is = 0;
	NG_Bullet_Pile1[0] = 0;
	u16 cadence = 0;
	u16 cadence2 = 0;

	while(1)
	{
		NG_Sprites[0].y++;

		NG_Joypad1_Read(&joypad1);

		if(joypad1.A == NG_BUTTON_PRESSED)
			cadence = 0;

		if( (cadence == 1) && (joypad1.A == NG_BUTTON_HOLD) )
		{
			NG_Fix_Print_String("A", 12, 12);	// We can't shoot anymore if this line is removed
			is = NG_Bullet_Pile1[0]&0xFF;
			NG_Bullets[is].x = ftoi7( (NG_Sprites[1].x+8) );
			NG_Bullets[is].y = ftoi7( (NG_Sprites[1].y-8) );
			NG_Bullets[is].vy = -ftoi7(2.5);

			is = NG_Bullet_Pile1[1]&0xFF;
			NG_Bullets[is].x = ftoi7( (NG_Sprites[1].x) );
			NG_Bullets[is].y = ftoi7( (NG_Sprites[1].y-8) );
			NG_Bullets[is].vy = -ftoi7(2.5);
			NG_Bullets[is].vx = -ftoi7(0.5);

			is = NG_Bullet_Pile1[2]&0xFF;
			NG_Bullets[is].x = ftoi7( (NG_Sprites[1].x+16) );
			NG_Bullets[is].y = ftoi7( (NG_Sprites[1].y-8) );
			NG_Bullets[is].vy = -ftoi7(2.5);
			NG_Bullets[is].vx = ftoi7(0.5);
		}

		if(cadence2 == 1)
		{
			is = NG_Bullet_Pile2[0]&0xFF;
			NG_Bullets[is].x = ftoi7( 64 );
			NG_Bullets[is].y = ftoi7( 100 );
			NG_Bullets[is].vy = ftoi7(1.0);

			is = NG_Bullet_Pile2[1]&0xFF;
			NG_Bullets[is].x = ftoi7( (64-4) );
			NG_Bullets[is].y = ftoi7( 100 );
			NG_Bullets[is].vy = ftoi7(1.0);
			NG_Bullets[is].vx = -ftoi7(0.15);

			is = NG_Bullet_Pile2[2]&0xFF;
			NG_Bullets[is].x = ftoi7( (64+4) );
			NG_Bullets[is].y = ftoi7( 100 );
			NG_Bullets[is].vy = ftoi7(1.0);
			NG_Bullets[is].vx = ftoi7(0.15);

			is = NG_Bullet_Pile2[3]&0xFF;
			NG_Bullets[is].x = ftoi7( (64-8) );
			NG_Bullets[is].y = ftoi7( 100 );
			NG_Bullets[is].vy = ftoi7(1.0);
			NG_Bullets[is].vx = -ftoi7(0.3);

			is = NG_Bullet_Pile2[4]&0xFF;
			NG_Bullets[is].x = ftoi7( (64+8) );
			NG_Bullets[is].y = ftoi7( 100 );
			NG_Bullets[is].vy = ftoi7(1.0);
			NG_Bullets[is].vx = ftoi7(0.3);
		}

		if(cadence2 == 2)
		{
			is = NG_Bullet_Pile2[0]&0xFF;
			NG_Bullets[is].x = ftoi7( 151 );
			NG_Bullets[is].y = ftoi7( 116 );
			NG_Bullets[is].vy = ftoi7(1.0);

			is = NG_Bullet_Pile2[1]&0xFF;
			NG_Bullets[is].x = ftoi7( (151-4) );
			NG_Bullets[is].y = ftoi7( 116 );
			NG_Bullets[is].vy = ftoi7(1.0);
			NG_Bullets[is].vx = -ftoi7(0.15);

			is = NG_Bullet_Pile2[2]&0xFF;
			NG_Bullets[is].x = ftoi7( (151+4) );
			NG_Bullets[is].y = ftoi7( 116 );
			NG_Bullets[is].vy = ftoi7(1.0);
			NG_Bullets[is].vx = ftoi7(0.15);

			is = NG_Bullet_Pile2[3]&0xFF;
			NG_Bullets[is].x = ftoi7( (151-8) );
			NG_Bullets[is].y = ftoi7( 116 );
			NG_Bullets[is].vy = ftoi7(1.0);
			NG_Bullets[is].vx = -ftoi7(0.3);

			is = NG_Bullet_Pile2[4]&0xFF;
			NG_Bullets[is].x = ftoi7( (151+8) );
			NG_Bullets[is].y = ftoi7( 116 );
			NG_Bullets[is].vy = ftoi7(1.0);
			NG_Bullets[is].vx = ftoi7(0.3);
		}

		if(cadence2 == 3)
		{
			is = NG_Bullet_Pile2[0]&0xFF;
			NG_Bullets[is].x = ftoi7( 238 );
			NG_Bullets[is].y = ftoi7( 100 );
			NG_Bullets[is].vy = ftoi7(1.0);

			is = NG_Bullet_Pile2[1]&0xFF;
			NG_Bullets[is].x = ftoi7( (238-4) );
			NG_Bullets[is].y = ftoi7( 100 );
			NG_Bullets[is].vy = ftoi7(1.0);
			NG_Bullets[is].vx = -ftoi7(0.15);

			is = NG_Bullet_Pile2[2]&0xFF;
			NG_Bullets[is].x = ftoi7( (238+4) );
			NG_Bullets[is].y = ftoi7( 100 );
			NG_Bullets[is].vy = ftoi7(1.0);
			NG_Bullets[is].vx = ftoi7(0.15);

			is = NG_Bullet_Pile2[3]&0xFF;
			NG_Bullets[is].x = ftoi7( (238-8) );
			NG_Bullets[is].y = ftoi7( 100 );
			NG_Bullets[is].vy = ftoi7(1.0);
			NG_Bullets[is].vx = -ftoi7(0.3);

			is = NG_Bullet_Pile2[4]&0xFF;
			NG_Bullets[is].x = ftoi7( (238+8) );
			NG_Bullets[is].y = ftoi7( 100 );
			NG_Bullets[is].vy = ftoi7(1.0);
			NG_Bullets[is].vx = ftoi7(0.3);
		}

		cadence++;
		cadence &= 0x7;

		cadence2++;
		cadence2 &= 0xF;

		if(joypad1.LEFT == NG_BUTTON_HOLD)
		{
			NG_Sprites[1].x-= 2;
		}

		if(joypad1.RIGHT == NG_BUTTON_HOLD)
		{
			NG_Sprites[1].x+= 2;
		}

		if(joypad1.UP == NG_BUTTON_HOLD)
		{
			NG_Sprites[1].y-= 2;
		}

		if(joypad1.DOWN == NG_BUTTON_HOLD)
		{
			NG_Sprites[1].y+= 2;
		}

		NG_Sprites[2].x = NG_Sprites[1].x;
		if(animb&3)
			NG_Sprites[2].y = NG_Sprites[1].y + 21;
		else
			NG_Sprites[2].y = 300;

		animb++;
		animb &= 7;

		NG_Sprites[4].x = NG_Sprites[3].x;
		NG_Sprites[4].y = NG_Sprites[3].y;

		NG_BulletPosition(0, 0, 4); // bullet ship
		NG_BulletPosition(1, 5, 18); //bullet ennemi

		//NG_Fix_Print_Cycles(33, 1);
		NG_Fix_Print_CPU(34, 0); //Print % CPU, X pos, Y pos
		NG_Wait_VBlank(); //Wait VBlank
		offset3(); //15% max
		NG_BulletUpdatePosition(idbullet, nbullet);

	}
}

void __attribute__((noinline)) NG_BulletPosition(u16 pile,u16 offset,u16 n)
{
	if(pile == 0)
		NG_arg1_u32 = (u32)NG_Bullet_Pile1;
	else
		NG_arg1_u32 = (u32)NG_Bullet_Pile2;

	NG_arg1_u16 = offset;
	NG_arg2_u16 = n;
	asm (
		"movem.l %d2-%d7,-(%a7)\n	"

		"lea    NG_Bullets,%a0\n	"
		"move.l NG_arg1_u32,%a1\n	"

		"move.w NG_arg1_u16,%d7\n	"
		"asl.w  #3,%d7\n	"
		"move.w %d7,%d0\n	"
		"asl.w  #4,%d0\n	"
		"add.w %d0,%a0\n	"


		"move.w #0x10,%d3\n	"
		"move.w #0x1,%d0\n	"
		"move.w #0xFFFF,%d4\n	"
		"move.w #0xA000,%d5\n	"
		"move.w #0x8000,%d6\n	"

		//----------------------------------
		"move.w NG_arg2_u16,%d2\n	"
		"addq.w #1,%d2\n	"
		"asl.w  #3,%d2\n	"
		"subi.w #1,%d2\n	"

		"NG_BulletPosition_loop:\n	"
			//Y position
			"move.w (%a0),%d1\n	"

			"btst  #15,%d1\n	"
			"bne NG_BulletPosition_off\n	"

			"add.w  10(%a0),%d1\n	"
			"move.w %d1,(%a0)\n	"
			"eor.w  %d4,%d1\n	"
			"andi.w #0xFF80,%d1\n	"
			"add.w  %d0,%d1\n	"
			"move.w %d1,12(%a0)\n	"

			//X Position
			"move.w  2(%a0),%d1\n	"

			"cmp.w   %d5,%d1\n	"
			"bcc NG_BulletPosition_off\n	"

			"add.w   8(%a0),%d1\n	"
			"move.w  %d1,2(%a0)\n	"
			"andi.w  #0xFF80,%d1\n	"
			"move.w  %d1,14(%a0)\n	"

			"NG_BulletPosition_off_ret:\n	"

			"add.w  #1,%d7\n	"
			"add.w  %d3,%a0\n	"
		"dbra.w %d2,NG_BulletPosition_loop\n	"
		"bra.w NG_BulletPosition_end\n	"

		//------
		"NG_BulletPosition_off:\n	"
		"move.w %d6,12(%a0)\n	"
		"move.w %d5,14(%a0)\n	"
		"clr.w %d1\n	"
		"move.w %d1,8(%a0)\n	"
		"move.w %d1,10(%a0)\n	"

		"move.w %d7,(%a1)+\n	"
		"bra.w NG_BulletPosition_off_ret\n	"
		//----------------------------------
		"NG_BulletPosition_end:\n	"

		"move.w  %d7,2(%a1)\n	"

		"movem.l (%a7)+,%d2-%d7\n	"
		);
}

void __attribute__((noinline)) NG_BulletUpdatePosition(u16 id,u16 n)
{
	NG_arg1_u16 = id;
	NG_arg2_u16 = n;
	asm (
		"movem.l %d2-%d3/%a2,-(%a7)\n	"

		"move.w  #0x200,VRAM_MOD\n	"

		"lea     NG_Bullets+12,%a0\n	"
		"lea     VRAM_ADDR,%a1\n	"
		"lea     VRAM_RW,%a2\n	"

		"move.w #SCB3+1,%d2\n	"
		"add.w  NG_arg1_u16,%d2\n	"

		"move.w  #0x10-2,%d3\n	"
		"move.w #0x1,%d0\n	"

		//----------------------------------
		"move.w NG_arg2_u16,%d1\n	"

		//SCB3
		"NG_BulletUpdatePosition_loop:\n	"
			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"
		"dbra.w %d1,NG_BulletUpdatePosition_loop\n	"


		//----------------------------------
		"movem.l (%a7)+,%d2-%d3/%a2\n	"
		);
}


void __attribute__((noinline)) NG_BulletUpdate(u16 id,u16 n)
{
	NG_arg1_u16 = id;
	NG_arg2_u16 = n;
	asm (
		"movem.l %d2-%d5/%a2,-(%a7)\n	"

		"move.w  #0x200,VRAM_MOD\n	"

		"lea     NG_Bullets,%a0\n	"
		"lea     VRAM_ADDR,%a1\n	"
		"lea     VRAM_RW,%a2\n	"

		"move.w #SCB3+1,%d2\n	"
		"add.w  NG_arg1_u16,%d2\n	"

		"move.w #0x10,%d3\n	"
		"move.w #0x1,%d0\n	"
		"move.w #0xFFFF,%d4\n	"

		//----------------------------------
		"move.w NG_arg2_u16,%d5\n	"
		"addq.w #1,%d5\n	"
		"asl.w  #3,%d5\n	"
		"subi.w #1,%d1\n	"

		//SCB3
		"NG_BulletUpdate_loop:\n	"
			"move.w %d2,(%a1)\n	"

			//Y position
			"move.w (%a0),%d1\n	"
			"eor.w  %d4,%d1\n	"
			"asl.w  #7,%d1\n	"
			"add.w  %d0,%d1\n	"
			"move.w %d1,(%a2)\n	"

			//X Position
			"move.w  2(%a0),%d1\n	"
			"asl.w   #7,%d1\n	"
			"move.w  %d1,(%a2)\n	"

			"add.w  %d3,%a0\n	"
			"add.w  %d0,%d2\n	"


		"dbra.w %d5,NG_BulletUpdate_loop\n	"


		//----------------------------------
		"movem.l (%a7)+,%d2-%d5/%a2\n	"
		);
}

void __attribute__((noinline)) NG_BulletUpdateTile(u16 id,u16 n)
{
	NG_arg1_u16 = id;
	NG_arg2_u16 = n;
	asm (
		"movem.l %d2-%d3/%a2,-(%a7)\n	"

		"move.w  #0x01,VRAM_MOD\n	"

		"lea     NG_Bullets+4,%a2\n	"
		"lea     VRAM_ADDR,%a0\n	"
		"lea     VRAM_RW,%a1\n	"

		"move.w  NG_arg1_u16,%d0\n	"
		"addq.w #1,%d0\n	"
		"asl.w  #6,%d0\n	"

		"move.w  #12,%d3\n	"

		"move.w NG_arg2_u16,%d1\n	"
		"addq.w #1,%d1\n	"
		"asl.w  #3,%d1\n	"
		"subi.w #1,%d1\n	"

		//SCB1
		"NG_BulletUpdateTile_loop:\n	"
			"move.w  %d0,(%a0)\n	" //8 cycles
			"addi.w  #0x40,%d0\n	" //8 cycles

			"move.w  (%a2)+,%d2\n	" //8 cycles
			"move.w  %d2,(%a1)\n	" //8 cycles
			NG_NOP //4 cycles

			"move.w  (%a2)+,%d2\n	"  //8 cycles
			"move.w  %d2,(%a1)\n	" //8 cycles

			"add.w  %d3,%a2\n	"  //4 cycles
		"dbra.w %d1,NG_BulletUpdateTile_loop\n	"

		//----------------------------------
		"movem.l (%a7)+,%d2-%d3/%a2\n	"
		);
}

