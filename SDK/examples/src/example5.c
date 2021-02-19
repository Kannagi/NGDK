#include <NGDK.h>
#include "data_ext.h"

#define MAP_WIDTH 128

extern u16 buffer_map_A[0x800*2];
extern u16 buffer_map_B[0x800*2];
extern u8 buffer_map_C[0x800*1];

void example5(void )
{
	NG_joypad joypad1 = NG_JOYPAD_INIT; //Init Joypad
	NG_Sprite_Clear_VRAM(); //Clear VRAM Sprite
	NG_Fix_Clear(); //Clear Fix

	NG_Backdrop_Color_Set(NG_COLOR(7, 7, 7)); //Backdrop color R, G, B (0-15)

	//MAP
	NG_Palette_Load(tile1_pal_data, 16); //Load palette, PAL : 16
	NG_Palette_Load(tile2_pal_data, 17);
	NG_Palette_Load(tile3_pal_data, 18);
	NG_Palette_Load(tile4_pal_data, 19);
	NG_Palette_Load(tile5_pal_data, 20);
	NG_Palette_Load(tile6A_pal_data, 21);
	NG_Palette_Load(tile6B_pal_data, 22);
	NG_Palette_Load(tile6C_pal_data, 23);
	NG_Palette_Load(tile6D_pal_data, 24);
	NG_Palette_Load(tile6E_pal_data, 25);
	NG_Palette_Load(tile6F_pal_data, 26);
	NG_Palette_Load(tile6G_pal_data, 27);
	NG_Palette_Load(tile6H_pal_data, 28);
	NG_Palette_Load(tile6I_pal_data, 29);
	NG_Palette_Load(tile7_pal_data, 30);
	NG_Palette_Load(tile8_pal_data, 31);
	NG_Palette_Load(tile9_pal_data, 32);
	NG_Palette_Load(tile10_pal_data, 33);
	NG_Palette_Load(tile11_pal_data, 34);

	//-----------fix--------------
	NG_Fix_Palette(NG_FIX_PAL(0)); //Select Pal for Print
	NG_Fix_Print_String("Hello Worlb !", 12, 0); //Print text, str, X pos, Y pos
	NG_Fix_Print_String("CPU:", 29, 0);
	NG_Fix_Print_u16(NGDK_VERSION, 0, 0); //Print variable u16, X pos, Y pos
	NG_Fix_Print_u16(NG_MAX_SPR, 0, 1);

	NG_Fix_Print_Free_Mem(0, 5);

	//-----------init sprite--------------
	NG_Sprite_Init(0, 0, 0, 32, 15, 0x0000, 0); //BG1
	NG_Sprite_Init(1, 0, 0, 32, 15, 0x0000, 0); //BG2

	//----Init Sprite VRAM------------
	NG_Sprite_VRAM_ID_Set(0x00);
	NG_Sprite_VRAM_Init(0); //BG1
	NG_Sprite_VRAM_Init(1);	//BG2
	NG_Sprite_Update_N(1); //Update position X, Y, MetaSPR, Zoom XY, 0-N

	//NG_Sprite_VRAM_ID_Get gives real ID, max 380
	NG_Fix_Print_u16(NG_Sprite_VRAM_ID_Get(), 0, 2);
	int i;
	NG_Background_Width_Set(MAP_WIDTH);

	u32 *map1 = (u32*)stage1_A_data;
	u32 *map2 = (u32*)stage1_B_data;

	for(i = 0; i < 20; i++)
		NG_Background_Vertical_Update(0, i, &map1[i]);

	for(i = 0; i < 20; i++)
		NG_Background_Vertical_Update(1, i, &map2[i]);

	NG_Wait_VBlank(); //Wait VBlank

	u16 camX = 0;

	while(1)
	{
		NG_Joypad1_Read(&joypad1);

		if(joypad1.LEFT == NG_BUTTON_HOLD)
		{
			if(camX > 0){
				camX -= 2;
				NG_Sprites[0].x = -camX + 8;
				NG_Background_Scroll_X(0, -1, map1);
				NG_Background_Scroll_X(1, -1, map2);
			}
		}

		if(joypad1.RIGHT == NG_BUTTON_HOLD)
		{
			if(camX < ((MAP_WIDTH-20) * 16)){
				camX += 2;
				NG_Sprites[0].x = -camX + 8;
				NG_Background_Scroll_X(0, 20, map1);
				NG_Background_Scroll_X(1, 20, map2);
			}
		}

		NG_Sprites[1].x = NG_Sprites[0].x;
		NG_Sprites[1].y = NG_Sprites[0].y;

		NG_Fix_Print_u16(NG_Sprites[0].x, 0, 2);

		NG_Wait_VBlank(); //Wait VBlank
		NG_Fix_Print_Cycles(33, 1);
		NG_Fix_Print_CPU(34, 0); //Print % CPU, X pos, Y pos
		NG_Fix_Print_u16(camX, 15, 15);
		NG_Fix_Print_h16(camX, 15, 16);
		NG_Sprite_Update_N(1);

	}
}
