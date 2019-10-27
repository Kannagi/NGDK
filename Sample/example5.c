#include <NGDK.h>
#include "data/data_ext.h"

extern u16 buffer_map_A[0x800*2];
extern u16 buffer_map_B[0x800*2];
extern u8 buffer_map_C[0x800*1];


void example5(void )
{
	NG_Joypads joypad1 = NG_JOYPAD_INIT; //Init Joypad
	NG_SpriteClearVRAM(); //Clear VRAM Sprite
	NG_ClearFix(); //Clear Fix

	NG_BGColor(NG_COLOR(7,7,7)); //Color BG R,G,B 0-15


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


	//-----------fix--------------
	NG_FixPal(FIXPAL(0)); //Select Pal for Print
	NG_Print("Hello Worlb !",12,0); //Print text , str , Pos X , Pos Y
	NG_Print("CPU:",29,0);
	NG_Printu16(NGDK_VERSION,0,0); //Print variable u16 , Pos X , Pos Y
	NG_Printu16(NG_MAX_SPR,0,1);

	NG_PrintFreeMem(0,5);

	//-----------init sprite--------------
	NG_SpriteInit(0,0,0,32,15,0x0000,0);  //BG1
	NG_SpriteInit(1,0,0,32,15,0x0000,0); //BG2


	//----Init Sprite VRAM------------
	NG_SpriteSetID(0x00);
	NG_SpriteUpdateInit(0); //BG1
	NG_SpriteUpdateInit(1);	//BG2
	NG_SpriteUpdateN(1); // Update position X,Y,MetaSPR , Zoom XY, 0-N

	// NG_SpriteGetID give ID actually , max 380
	NG_Printu16(NG_SpriteGetID(),0,2);
	int i;
	NG_BGWidthSet(128);
	u16 sizemap = (128*14);

	NG_Map_Decompress(stage1_A_data,buffer_map_A,sizemap);
	NG_Map_Decompress(stage1_B_data,buffer_map_B,sizemap);

	u32 *map1 = (u32*)buffer_map_A;
	u32 *map2 = (u32*)buffer_map_B;

	for(i = 0;i < 20;i++)
		NG_SpriteUpdateMapVertical(&map1[i],0,i);

	for(i = 0;i < 20;i++)
		NG_SpriteUpdateMapVertical(&map2[i],1,i);


	NG_WaitVBlank(); //Wait VBlank
	while(1)
	{
		NG_Joypad1(&joypad1);


		if(joypad1.LEFT == NG_BUTTON_DOWNW)
		{
			NG_Sprite[0].x+=2;
			NG_BGScrollx(map1,-1,0);
			NG_BGScrollx(map2,-1,1);
		}

		if(joypad1.RIGHT == NG_BUTTON_DOWNW)
		{
			NG_Sprite[0].x-=2;
			NG_BGScrollx(map1,20,0);
			NG_BGScrollx(map2,20,1);
		}



		NG_Sprite[1].x = NG_Sprite[0].x;
		NG_Sprite[1].y = NG_Sprite[0].y;

		NG_Printu16(NG_Sprite[0].x,0,2);

		NG_WaitVBlank(); //Wait VBlank
		NG_Cycle(33,1);
		NG_PrintCPU(34,0); //Print %CPU , Pos X , Pos Y ;
		NG_SpriteUpdateN(1);

	}
}
