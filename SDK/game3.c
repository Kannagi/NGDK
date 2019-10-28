
#include "NGDK/NGDK.h"
#include "data/data_ext.h"

extern const u16 palette_ng[32];

void offset3(void)
{
	NG_SpriteUpdateN(4);
}

void __attribute__((optimize("-O3"),noinline)) game3(void )
{
	NG_ClearFix(); //Clear Fix
	NG_Joypads joypad1 = NG_JOYPAD_INIT; //Init Joypad
	NG_SpriteClearVRAM(); //Clear VRAM Sprite


	NG_BGColor(NG_COLOR(7,7,7)); //Color BG R,G,B 0-15

	//Load Palette (FIX 0-15)
	NG_LoadPal(palette_ng,0); //Load palette , PAL : 0
	RW_REGISTER_U16(0x3C0006) = 0x700;

	//MAP

	NG_LoadPal(BGD_pal_data,16);

	NG_LoadPal(ship_pal_data,0x80);
	NG_LoadPal(booster_pal_data,0x81);
	NG_LoadPal(bullet_pal_data,0x82);
	NG_LoadPal(bullet2_pal_data,0x83);
	NG_LoadPalN(Boss_pal_data,0x84,1);

	//-----------fix--------------
	NG_FixPal(FIXPAL(0)); //Select Pal for Print
	NG_Print("Dobonpachi !",12,0); //Print text , str , Pos X , Pos Y
	NG_Print("CPU:",29,0);
	NG_Printu16(NGDK_VERSION,0,0); //Print variable u16 , Pos X , Pos Y
	//NG_Printu16(NG_MAX_SPR,0,1);

	//NG_PrintFreeMem(0,5);

	NG_Printu8(255,0,5);
	NG_Prints8(128,0,6);
	NG_Printh8(100,0,7);

	//-----------init sprite--------------
	NG_SpriteInit(0,0,0,20,32,BGD_index,0x1000);  //BG1

	NG_SpriteInit(1,130,130,2,2,ship_index,NG_FLAG(0x80,0,0,0)); //Sprite , w,h,tile,flag(pal,tileMSB,auto-anim,flip)
	NG_SpriteInit(2,130,130,2,1,booster_index,NG_FLAG(0x81,0,0,0));

	NG_SpriteInitHicolor(3,40,0,14,8,Boss_index,NG_FLAG(0x84,0,0,0));

	//----Init Sprite VRAM------------
	NG_SpriteSetID(0x00);
	NG_SpriteUpdateInit(0); //BG1

	NG_SpriteUpdateInit(3);
	NG_SpriteUpdateInit(4);

	NG_SpriteUpdateInit(1);
	NG_SpriteUpdateInit(2);
	NG_SpriteUpdateN(4); // Update position X,Y,MetaSPR , Zoom XY, 0-N

	// NG_SpriteGetID give ID actually , max 380
	//NG_Printu16(NG_SpriteGetID(),0,2);
	u16 i;

	for(i = 0;i < 40;i++)
	{
		NG_Bullet[i].x = 0;
		NG_Bullet[i].y = ftoi7(256);
		NG_Bullet[i].tile = bullet_index;
		NG_Bullet[i].flag = NG_FLAG(0x82,0,0,0);
		NG_Bullet[i].vx = 0;
		NG_Bullet[i].vy = 0;
	}

	for(i = 40;i < NG_MAX_BULLET;i++)
	{
		NG_Bullet[i].x = 0;
		NG_Bullet[i].y = ftoi7(256);
		NG_Bullet[i].tile = bullet2_index;
		NG_Bullet[i].flag = NG_FLAG(0x83,0,1,0);
		NG_Bullet[i].vx = 0;
		NG_Bullet[i].vy = 0;
	}



	u16 animb = 0;
	u16 nbullet = 31;
	u16 idbullet = 124;
	NG_BulletUpdateTile(idbullet,nbullet);

	NG_WaitVBlank(); //Wait VBlank
	u16 is = 0;
	NG_Bullet_Pile1[0] = 0;
	u16 cadence = 0;
	u16 cadence2 = 0;

	s8 var = 0;
	while(1)
	{
		var++;

		NG_Sprite[0].y++;

		NG_Joypad1(&joypad1);

		if(joypad1.B == NG_BUTTON_DOWN)
			NG_SetYM2612(20);
		if(joypad1.A == NG_BUTTON_DOWN)
			cadence = 0;

		if( (cadence == 1) && (joypad1.A == NG_BUTTON_DOWNW) )
		{

			is = NG_Bullet_Pile1[0]&0xFF;
			NG_Bullet[is].x = ftoi7( (NG_Sprite[1].x+8) );
			NG_Bullet[is].y = ftoi7( (NG_Sprite[1].y-8) );
			NG_Bullet[is].vy = -ftoi7(2.5);

			is = NG_Bullet_Pile1[1]&0xFF;
			NG_Bullet[is].x = ftoi7( (NG_Sprite[1].x) );
			NG_Bullet[is].y = ftoi7( (NG_Sprite[1].y-8) );
			NG_Bullet[is].vy = -ftoi7(2.5);
			NG_Bullet[is].vx = -ftoi7(0.5);

			is = NG_Bullet_Pile1[2]&0xFF;
			NG_Bullet[is].x = ftoi7( (NG_Sprite[1].x+16) );
			NG_Bullet[is].y = ftoi7( (NG_Sprite[1].y-8) );
			NG_Bullet[is].vy = -ftoi7(2.5);
			NG_Bullet[is].vx = ftoi7(0.5);
		}

		if(cadence2 == 1)
		{
			is = NG_Bullet_Pile2[0]&0xFF;
			NG_Bullet[is].x = ftoi7( 64 );
			NG_Bullet[is].y = ftoi7( 100 );
			NG_Bullet[is].vy = ftoi7(1.0);

			is = NG_Bullet_Pile2[1]&0xFF;
			NG_Bullet[is].x = ftoi7( (64-4) );
			NG_Bullet[is].y = ftoi7( 100 );
			NG_Bullet[is].vy = ftoi7(1.0);
			NG_Bullet[is].vx = -ftoi7(0.15);

			is = NG_Bullet_Pile2[2]&0xFF;
			NG_Bullet[is].x = ftoi7( (64+4) );
			NG_Bullet[is].y = ftoi7( 100 );
			NG_Bullet[is].vy = ftoi7(1.0);
			NG_Bullet[is].vx = ftoi7(0.15);

			is = NG_Bullet_Pile2[3]&0xFF;
			NG_Bullet[is].x = ftoi7( (64-8) );
			NG_Bullet[is].y = ftoi7( 100 );
			NG_Bullet[is].vy = ftoi7(1.0);
			NG_Bullet[is].vx = -ftoi7(0.3);

			is = NG_Bullet_Pile2[4]&0xFF;
			NG_Bullet[is].x = ftoi7( (64+8) );
			NG_Bullet[is].y = ftoi7( 100 );
			NG_Bullet[is].vy = ftoi7(1.0);
			NG_Bullet[is].vx = ftoi7(0.3);
		}

		if(cadence2 == 2)
		{
			is = NG_Bullet_Pile2[0]&0xFF;
			NG_Bullet[is].x = ftoi7( 151 );
			NG_Bullet[is].y = ftoi7( 116 );
			NG_Bullet[is].vy = ftoi7(1.0);

			is = NG_Bullet_Pile2[1]&0xFF;
			NG_Bullet[is].x = ftoi7( (151-4) );
			NG_Bullet[is].y = ftoi7( 116 );
			NG_Bullet[is].vy = ftoi7(1.0);
			NG_Bullet[is].vx = -ftoi7(0.15);

			is = NG_Bullet_Pile2[2]&0xFF;
			NG_Bullet[is].x = ftoi7( (151+4) );
			NG_Bullet[is].y = ftoi7( 116 );
			NG_Bullet[is].vy = ftoi7(1.0);
			NG_Bullet[is].vx = ftoi7(0.15);

			is = NG_Bullet_Pile2[3]&0xFF;
			NG_Bullet[is].x = ftoi7( (151-8) );
			NG_Bullet[is].y = ftoi7( 116 );
			NG_Bullet[is].vy = ftoi7(1.0);
			NG_Bullet[is].vx = -ftoi7(0.3);

			is = NG_Bullet_Pile2[4]&0xFF;
			NG_Bullet[is].x = ftoi7( (151+8) );
			NG_Bullet[is].y = ftoi7( 116 );
			NG_Bullet[is].vy = ftoi7(1.0);
			NG_Bullet[is].vx = ftoi7(0.3);
		}

		if(cadence2 == 3)
		{
			is = NG_Bullet_Pile2[0]&0xFF;
			NG_Bullet[is].x = ftoi7( 238 );
			NG_Bullet[is].y = ftoi7( 100 );
			NG_Bullet[is].vy = ftoi7(1.0);

			is = NG_Bullet_Pile2[1]&0xFF;
			NG_Bullet[is].x = ftoi7( (238-4) );
			NG_Bullet[is].y = ftoi7( 100 );
			NG_Bullet[is].vy = ftoi7(1.0);
			NG_Bullet[is].vx = -ftoi7(0.15);

			is = NG_Bullet_Pile2[2]&0xFF;
			NG_Bullet[is].x = ftoi7( (238+4) );
			NG_Bullet[is].y = ftoi7( 100 );
			NG_Bullet[is].vy = ftoi7(1.0);
			NG_Bullet[is].vx = ftoi7(0.15);

			is = NG_Bullet_Pile2[3]&0xFF;
			NG_Bullet[is].x = ftoi7( (238-8) );
			NG_Bullet[is].y = ftoi7( 100 );
			NG_Bullet[is].vy = ftoi7(1.0);
			NG_Bullet[is].vx = -ftoi7(0.3);

			is = NG_Bullet_Pile2[4]&0xFF;
			NG_Bullet[is].x = ftoi7( (238+8) );
			NG_Bullet[is].y = ftoi7( 100 );
			NG_Bullet[is].vy = ftoi7(1.0);
			NG_Bullet[is].vx = ftoi7(0.3);
		}

		cadence++;
		cadence &= 0x7;

		cadence2++;
		cadence2 &= 0xF;

		if(joypad1.LEFT == NG_BUTTON_DOWNW)
		{
			NG_Sprite[1].x-= 2;
		}

		if(joypad1.RIGHT == NG_BUTTON_DOWNW)
		{
			NG_Sprite[1].x+= 2;
		}

		if(joypad1.UP == NG_BUTTON_DOWNW)
		{
			NG_Sprite[1].y-= 2;
		}

		if(joypad1.DOWN == NG_BUTTON_DOWNW)
		{
			NG_Sprite[1].y+= 2;
		}

		NG_Sprite[2].x = NG_Sprite[1].x;
		if(animb&3)
			NG_Sprite[2].y = NG_Sprite[1].y + 21;
		else
			NG_Sprite[2].y = 300;

		animb++;
		animb &= 7;

		NG_Sprite[4].x = NG_Sprite[3].x;
		NG_Sprite[4].y = NG_Sprite[3].y;

		NG_BulletPosition(0,0,4); // bullet ship
		NG_BulletPosition(1,5,18); //bullet ennemi

		//RW_REGISTER_U8(0x320000);
		NG_Printh8(RW_REGISTER_U8(0x320000),0,22);

		//NG_Cycle(33,1);
		NG_PrintCPU(34,0); //Print %CPU , Pos X , Pos Y ;
		NG_WaitVBlank(); //Wait VBlank
		offset3(); //15% max
		NG_BulletUpdatePosition(idbullet,nbullet);

	}
}
