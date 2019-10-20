#include <NGDK.h>
#include "data/data_ext.h"


void example5(void)
{
	NG_ClearFix(); //Clear Fix
	NG_SpriteClearVRAM(); //Clear VRAM Sprite

	NG_BGColor(NG_COLOR(7,7,7)); //Color BG R,G,B 0-15

	//-----------fix--------------
	NG_FixPal(FIXPAL(0)); //Select Pal for Print
	NG_Print("Hello Worlb !",12,0); //Print text , str , Pos X , Pos Y
	NG_Print("CPU:",29,0);
	NG_Printu16(NGDK_VERSION,0,0); //Print variable u16 , Pos X , Pos Y
	NG_Printu16(NG_MAX_SPR,0,1);

	NG_PrintFreeMem(0,5); //Indicates available memory

	//Sprite
	NG_LoadPal(MarcoH1_pal_data,0x80);
	NG_SpriteInit(&NG_Sprite[0],2,2,Map_tile_SPR[0],NG_FLAG(0x80,0,0,0));

	NG_Sprite[0].x += 100;
	NG_Sprite[0].y += 100;

	//BG
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
	NG_SpriteInit(&NG_Sprite[1],32,15,0x0000,0); //BG1
	NG_SpriteInit(&NG_Sprite[2],32,15,0x0000,0); //BG2

	//----Init Sprite VRAM------------
	NG_SpriteSetID(0x00);
	NG_SpriteUpdateInit(1); //BG1
	NG_SpriteUpdateInit(2);	//BG2
	NG_SpriteUpdateInit(0); //Init ID Sprite 0 + meta sprite + Upload
	NG_SpriteUpdateN(2); // Update position X,Y,MetaSPR , Zoom XY, 0-N

	// NG_SpriteGetID give ID actually , max 380
	NG_Printu16(NG_SpriteGetID(),0,2);

	int i;

	NG_BGWidthSet(128);

	//Draw BG1
	for(i = 0;i < 20;i++)
		NG_SpriteUpdateMapVertical(&stage1_A_data[4*i],1,i);

	//Draw BG2
	for(i = 0;i < 20;i++)
		NG_SpriteUpdateMapVertical(&stage1_B_data[4*i],2,i);

	NG_Joypads joypad1 = NG_JOYPAD_INIT; //Init Joypad

	u16 flip = 0;
	NG_Anim anim;
	NG_Anim_Init(&anim); //init anim
	u16 speedx = ftoi2(1.5); //convert double 1.5 to fix16 14.2
	u16 posx = ftoi2(100.5);


	while(1)
	{
		NG_Joypad1(&joypad1); //read input joypad

		if(joypad1.LEFT == NG_BUTTON_DOWNW)
			posx -= speedx;

		if(joypad1.RIGHT == NG_BUTTON_DOWNW)
			posx += speedx;

		if(joypad1.UP == NG_BUTTON_DOWNW)
			NG_Sprite[0].y -= 1;

		if(joypad1.DOWN == NG_BUTTON_DOWNW)
			NG_Sprite[0].y += 1;

		NG_Sprite[0].x = itoi2(posx);  //convert fix16 14.2 to u16

		// Dezoom H/V Control
		if(joypad1.C == NG_BUTTON_DOWN)
			NG_Sprite[0].zoom += 0x110;

		if(joypad1.D == NG_BUTTON_DOWN)
			NG_Sprite[0].zoom -= 0x110;

		NG_Sprite[0].zoom = NG_Sprite[0].zoom&0xFFF;
		NG_SpriteUpdateZoom(0); // Update Zoom X

		if(joypad1.A == NG_BUTTON_DOWN)
			flip = !flip;

		NG_Sprite[0].flag = NG_FLAG(0x80,0,0,flip);
		NG_SpriteUpdateTile(0); // Update Tile Sprite 0

		NG_Anim_Play_Fast(&anim,10,4); // cadence , n anim max
		NG_Sprite[0].tile = Map_tile_SPR[0]+(anim.i*4); //indicate tile in ROM C1/C2
		NG_SpriteUpdateN(2);

		NG_PrintCPU(34,0); //Print %CPU , Pos X , Pos Y ;
		NG_Cycle(32,1); //Print Cycle

		NG_WaitVBlank(); //Wait VBlank
	}
}


