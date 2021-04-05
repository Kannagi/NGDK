#include <NGDK.h>
#include "data_ext.h"

void example4(void)
{
	NG_Sprite_Clear_VRAM(); //Clear VRAM Sprite
	NG_Fix_Clear(); //Clear Fix

	NG_Backdrop_Color_Set(NG_COLOR(7, 7, 7)); //Backdrop color R, G, B (0-15)

	//-----------fix--------------
	NG_Fix_Palette(NG_FIX_PAL(0)); //Select Pal for Print
	NG_Fix_Print_String("Hello Worlb !", 12, 0); //Print text, str, X pos, Y pos
	NG_Fix_Print_String("CPU:", 29, 0);
	NG_Fix_Print_u16(NGDK_VERSION, 0, 0); //Print variable u16, X pos, Y pos
	NG_Fix_Print_u16(NG_MAX_SPR, 0, 1);

	NG_Fix_Print_Free_Mem(0, 5); //Indicates available memory

	//Sprite
	NG_Palette_Load(MarcoH1_pal_data, 0x80);
	NG_Sprite_Init(0, 100, 100, 2, 2, MarcoH1_index, NG_SPR_FLAGS(0x80, 0, 0, 0));


	//----Init Sprite VRAM------------
	NG_Sprite_VRAM_ID_Set(0x00);
	NG_Sprite_VRAM_Init(0); //Init ID Sprite 0 + meta sprite + Upload
	NG_Sprite_Update_N(2); //Update position X, Y, MetaSPR, Zoom XY, 0-N

	//NG_Sprite_VRAM_ID_Get gives real ID, max 380
	NG_Fix_Print_u16(NG_Sprite_VRAM_ID_Get(), 0, 2);

	int i;

	NG_joypad joypad1 = NG_JOYPAD_INIT; //Init Joypad

	u16 flip = 0;
	NG_Animation anim;
	NG_Anim_Init(&anim); //init anim
	u16 speedx = ftoi2(1.5); //convert double 1.5 to fix16 14.2
	u16 posx = ftoi2(100.5);

	while(1)
	{
		NG_Joypad1_Read(&joypad1); //read input joypad

		if(joypad1.LEFT == NG_BUTTON_HOLD)
			posx -= speedx;

		if(joypad1.RIGHT == NG_BUTTON_HOLD)
			posx += speedx;

		if(joypad1.UP == NG_BUTTON_HOLD)
			NG_Sprites[0].y -= 1;

		if(joypad1.DOWN == NG_BUTTON_HOLD)
			NG_Sprites[0].y += 1;

		NG_Sprites[0].x = itoi2(posx);  //convert fix16 14.2 to u16

		// Dezoom H/V Control
		if(joypad1.C == NG_BUTTON_PRESSED)
			NG_Sprites[0].zoom += 0x110;

		if(joypad1.D == NG_BUTTON_PRESSED)
			NG_Sprites[0].zoom -= 0x110;

		NG_Sprites[0].zoom = NG_Sprites[0].zoom & 0xFFF;
		NG_Sprite_Zoom_Update(0); //Update Zoom X

		if(joypad1.A == NG_BUTTON_PRESSED)
			flip = !flip;

		NG_Sprites[0].flags = NG_SPR_FLAGS(0x80, 0, 0, flip);
		NG_Sprite_Tiles_Update(0); //Update Tile Sprite 0

		NG_Anim_Play_Fast(&anim, 10, 4); // cadence, n anim max
		NG_Sprites[0].tile = MarcoH1_index + (anim.frame * 4); //indicate tile in ROM C1/C2
		NG_Sprite_Update_N(0);

		NG_Fix_Print_CPU(34, 0); //Print % CPU, X pos, Y pos
		NG_Fix_Print_Cycles(32, 1); //Print executed cycles count

		NG_Wait_VBlank(); //Wait VBlank
	}
}


