
#include "NGDK/NGDK.h"

const u16 palette_ng[32] = { 0x0f0f,0x0eee,0x0ddd,0x0ccc,0x0bbb,0x0aaa,0x0999,0x0888,0x010f,0x0f00,0x00ff,0x0f0f,0x0f0f,0x0f0f,0x0f0f,0x0000 };

const unsigned char hero_pal_data[] ={
	0x5f, 0x0f, 0x7f, 0xff, 0x7a, 0xaa, 0x05, 0x55,
	0x20, 0xf0, 0x00, 0x50, 0x0f, 0x50, 0x05, 0x00,
	0x2f, 0xa5, 0x00, 0x00, 0x0f, 0x00, 0x2f, 0xf0,
	0x57, 0x7f,
};

#include "data/data.h"

const u8 tile[16] ={0x00, 0x01, 0x10,0x00, 0x02,0x00, 0x10,0x00};

void  game(void )
{
	NG_Joypads joypad1 = NG_JOYPAD_INIT; //Init Joypad
	NG_SpriteClearVRAM(); //Clear VRAM Sprite
	NG_ClearFix(); //Clear Fix

	NG_BGColor(NG_COLOR(7,7,7)); //Color BG R,G,B 0-15

	//Load Palette (FIX 0-15)
	NG_LoadPal(palette_ng,0); //Load palette , PAL : 0

	//MAP

	NG_LoadPal(tile1_pal_data,16); //Load palette , PAL : 16
	NG_LoadPal(tile2_pal_data,17);
	NG_LoadPal(tile3_pal_data,18);
	NG_LoadPal(tile4_pal_data,19);
	NG_LoadPal(tile5_pal_data,20);
	NG_LoadPal(tile6A_pal_data,21);
	NG_LoadPal(tile6B_pal_data,22);
	NG_LoadPal(tile6C_pal_data,23);
	NG_LoadPal(tile6D_pal_data,24);
	NG_LoadPal(tile6E_pal_data,25);
	NG_LoadPal(tile6F_pal_data,26);
	NG_LoadPal(tile6G_pal_data,27);
	NG_LoadPal(tile6H_pal_data,28);
	NG_LoadPal(tile6I_pal_data,29);
	NG_LoadPal(tile7_pal_data,30);
	NG_LoadPal(tile8_pal_data,31);
	NG_LoadPal(tile9_pal_data,32);
	NG_LoadPal(tile10_pal_data,33);
	NG_LoadPal(tile11_pal_data,34);

	NG_LoadPal(MarcoLPal_pal_data,0x80);
	NG_LoadPal(MarcoH1_pal_data,0x81);


	//-----------fix--------------
	NG_FixPal(FIXPAL(0)); //Select Pal for Print
	NG_Print("Hello Worlb !",12,0); //Print text , str , Pos X , Pos Y
	NG_Print("CPU:",29,0);
	NG_Printu16(NGDK_VERSION,0,0); //Print variable u16 , Pos X , Pos Y
	//NG_Printu16(NG_MAX_SPR,0,1);

	//NG_PrintFreeMem(0,5);

	//-----------init sprite--------------
	NG_SpriteInit(&NG_Sprite[0],32,15,0x0000,0);  //BG1
	NG_SpriteInit(&NG_Sprite[1],32,15,0x0000,NG_FLAG(0x10,0,0,0)); //BG2

	NG_SpriteInit(&NG_Sprite[2],2,2,Map_tile_SPR[1],NG_FLAG(0x80,0,0,0)); //Sprite , w,h,tile,flag(pal,tileMSB,auto-anim,flip)
	NG_SpriteInit(&NG_Sprite[3],2,2,Map_tile_SPR[0],NG_FLAG(0x81,0,0,0));

	NG_Sprite[2].x += 100;
	NG_Sprite[2].y += 100;

	//----Init Sprite VRAM------------
	NG_SpriteSetID(0x00);
	NG_SpriteUpdateInit(0); //BG1
	NG_SpriteUpdateInit(1);	//BG2
	NG_SpriteUpdateInit(2); //Init ID Sprite 2 + meta sprite + Upload
	NG_SpriteUpdateInit(3);
	NG_SpriteUpdateN(3); // Update position X,Y,MetaSPR , Zoom XY, 0-N

	// NG_SpriteGetID give ID actually , max 380
	//NG_Printu16(NG_SpriteGetID(),0,2);

	int i;

	NG_BGWidthSet(128);
/*
	for(i = 0;i < 14;i++)
	{
		NG_SpriteUpdateMapHorizontal(&stage1_data[128*4*i],1,i);
	}

*/

	for(i = 0;i < 20;i++)
		NG_SpriteUpdateMapVertical(&stage1_A_data[4*i],0,i);


	for(i = 0;i < 20;i++)
		NG_SpriteUpdateMapVertical(&stage1_B_data[4*i],1,i);

	NG_Anim anim1;
	NG_Anim_Init(&anim1);

	NG_Anim anim2;
	NG_Anim_Init(&anim2);

	u16 x =100,y = (16*9)+8;
	u16 mx,my;
	u16 speedx = ftoi2(1.5);
	s16 bgx = 0;
	u16 absx = x;
	u16 ianim = 0;

	while(1)
	{
		NG_Joypad1(&joypad1);

		ianim = 0;
		mx = 3;
		my = 8;
		if(NG_Sprite[2].flag&1)
			mx = -4;
		if(joypad1.LEFT == NG_BUTTON_DOWNW)
		{
			x -= speedx;
			absx -= speedx;
			NG_Sprite[2].flag |= 0x1;
			NG_Sprite[3].flag |= 0x1;
			mx = -8;
			my = 12;
			ianim = 1;
		}


		if(joypad1.RIGHT == NG_BUTTON_DOWNW)
		{
			x += speedx;
			absx += speedx;
			NG_Sprite[2].flag &= 0xFFFE;
			NG_Sprite[3].flag &= 0xFFFE;
			mx = 5;
			my = 12;
			ianim = 1;
		}



		if(absx > ftoi2(660) )
			y = (16*11)+8;
		else
			y = (16*9)+8;



		if(x > ftoi2(180) )
		{
			x = ftoi2(180);

			NG_BGScrollx(stage1_A_data,20,0);
			NG_BGScrollx(stage1_B_data,20,1);

			bgx -= speedx;
		}

		if(x < ftoi2(120)  )
		{
			if(bgx != 0)
				x = ftoi2(120);
			else
			{
				if(x < ftoi2(4))
				{
					x = ftoi2(4);
					absx = ftoi2(4);
				}

			}

			NG_BGScrollx(stage1_A_data,-1,0);
			NG_BGScrollx(stage1_B_data,-1,1);
			bgx += speedx;
		}

		u16 tmpx = itoi2(x);

		NG_Sprite[2].x = tmpx;
		NG_Sprite[3].x = tmpx+mx;

		NG_Sprite[2].y = y;
		NG_Sprite[3].y = y-my;

		if(bgx > 0) bgx = 0;
		tmpx = itoi2(bgx);

		NG_Sprite[0].x = tmpx;
		NG_Sprite[1].x = tmpx;


		NG_Prints16(itoi2(absx),0,1);
/*
		if(joypad1->LEFT == NG_BUTTON_DOWNW)
		{
			NG_BGScrollx(stage1_0_data,-1,0);
			NG_Sprite[0].x += 2;

			NG_BGScrollx(stage1_1_data,-1,1);
			NG_Sprite[1].x += 2;
		}


		if(joypad1->RIGHT == NG_BUTTON_DOWNW)
		{
			NG_BGScrollx(stage1_0_data,20,0);
			NG_Sprite[0].x -= 2;

			NG_BGScrollx(stage1_1_data,20,1);
			NG_Sprite[1].x -= 2;
		}
*/


		//NG_Printu16(NG_Sprite[1].x ,0,1);

/*
		if(joypad1.LEFT == NG_BUTTON_DOWNW)
			NG_Sprite[2].x -= 1;

		if(joypad1.RIGHT == NG_BUTTON_DOWNW)
			NG_Sprite[2].x += 1;

		if(joypad1.UP == NG_BUTTON_DOWNW)
			NG_Sprite[2].y -= 1;

		if(joypad1.DOWN == NG_BUTTON_DOWNW)
			NG_Sprite[2].y += 1;

		if(joypad1->A == NG_BUTTON_DOWN)
			NG_Sprite[2].tile += 1;

		if(joypad1->C == NG_BUTTON_DOWN)
			NG_Sprite[2].zoom += 0x110;

		if(joypad1->D == NG_BUTTON_DOWN)
			NG_Sprite[2].zoom -= 0x110;

		NG_Sprite[2].zoom = NG_Sprite[2].zoom&0xFFF;
*/
		NG_PrintCPU(34,0); //Print %CPU , Pos X , Pos Y ;
		//NG_Cycle(32,1);


		//Higb
		NG_Anim_Play_Fast(&anim1,10,2);
		NG_Sprite[3].tile = Map_tile_SPR[0]+(anim1.i*8);
		NG_SpriteUpdateTile(3); // Update tile

		//Low
		if(ianim == 1)
		{
			NG_Anim_Play_Fast(&anim2,5,18);
			NG_Sprite[2].tile = Map_tile_SPR[2]+(anim2.i*4);

		}else
		{
			anim2.i = 0;
			anim2.itmp = 0;
			NG_Sprite[2].tile = Map_tile_SPR[1];
		}


		NG_SpriteUpdateTile(2); // Update tile


		//NG_SpriteUpdateZoom(2); // Update Zoom X

		//NG_SpriteUpdateTileHorizontal(tile,2);

		NG_SpriteUpdateN(3);

		NG_WaitVBlank(); //Wait VBlank

	}
}

int __attribute__((optimize("-O0"), noinline)) main()
{
	game();
	return 0;
}

