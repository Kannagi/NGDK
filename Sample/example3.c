#include <NGDK.h>
#include "data/data_ext.h"


void example3(void)
{
	NG_SpriteClearVRAM(); //Clear VRAM Sprite
	NG_ClearFix(); //Clear Fix

	NG_BGColor(NG_COLOR(7,7,7)); //Color BG R,G,B 0-15

	//-----------fix--------------
	NG_FixPal(FIXPAL(0)); //Select Pal for Print
	NG_Print("Hello Worlb !",12,0); //Print text , str , Pos X , Pos Y
	NG_Print("CPU:",29,0);
	NG_Printu16(NGDK_VERSION,0,0); //Print variable u16 , Pos X , Pos Y
	NG_Printu16(NG_MAX_SPR,0,1);

	NG_PrintFreeMem(0,5); //Indicates available memory

	//Sprite
	NG_LoadPal(MarcoH1_pal_data,16);
	NG_SpriteInit(0,100,100,2,2,MarcoH1_index,NG_FLAG(16,0,0,0));


	//----Init Sprite VRAM------------
	NG_SpriteSetID(0x00);
	NG_SpriteUpdateInit(0); //Init ID Sprite 0 + meta sprite + Upload
	NG_SpriteUpdateN(0); // Update position X,Y,MetaSPR , Zoom XY, 0-N

	// NG_SpriteGetID give ID actually , max 380
	NG_Printu16(NG_SpriteGetID(),0,2);

	NG_Joypads joypad1 = NG_JOYPAD_INIT; //Init Joypad

	u16 flip = 0;
	while(1)
	{
		NG_Joypad1(&joypad1); //read input joypad

		if(joypad1.LEFT == NG_BUTTON_DOWNW)
			NG_Sprite[0].x -= 1;

		if(joypad1.RIGHT == NG_BUTTON_DOWNW)
			NG_Sprite[0].x += 1;

		if(joypad1.UP == NG_BUTTON_DOWNW)
			NG_Sprite[0].y -= 1;

		if(joypad1.DOWN == NG_BUTTON_DOWNW)
			NG_Sprite[0].y += 1;

		// Dezoom H/V Control
		if(joypad1.C == NG_BUTTON_DOWN)
			NG_Sprite[0].zoom += 0x110;

		if(joypad1.D == NG_BUTTON_DOWN)
			NG_Sprite[0].zoom -= 0x110;

		NG_Sprite[0].zoom = NG_Sprite[0].zoom&0xFFF;
		NG_SpriteUpdateZoom(0); // Update Zoom X

		if(joypad1.A == NG_BUTTON_DOWN)
			flip = !flip;

		NG_Sprite[0].flag = NG_FLAG(16,0,0,flip);
		NG_SpriteUpdateTile(0); // Update Tile Sprite 0

		NG_SpriteUpdateN(0);

		NG_PrintCPU(34,0); //Print %CPU , Pos X , Pos Y ;
		NG_Cycle(32,1); //Print Cycle

		NG_WaitVBlank(); //Wait VBlank

	}
}

